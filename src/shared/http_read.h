#ifndef HTTP_READ_H
#define HTTP_READ_H


#include "http_write.h"
#include "decompress.h"
#include "file_ops.h"
#include "database_read.h"
#include "database_write.h"
#include "trimws.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h> 
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Exception.h>


inline Poco::Net::HTTPResponse fetchJsonData(const std::string& url, std::stringstream& responseData) {
    try {
        Poco::URI uri(url);
        
        std::shared_ptr<Poco::Net::HTTPClientSession> session;
        
        if (uri.getScheme() == "https") {
            session = std::make_shared<Poco::Net::HTTPSClientSession>(uri.getHost(), uri.getPort());
        } else {
            session = std::make_shared<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());
        }

        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);
        request.set("Accept", "application/json");
        request.set("Accept-Encoding", "gzip, deflate, br, zstd");
        request.set("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:132.0) Gecko/20100101 Firefox/132.0");
        request.set("Accept-Language", "en-US,en;q=0.5");
        request.set("Connection", "keep-alive");
        request.set("DNT", "1");
        request.set("Host", uri.getHost());
        request.set("Priority", "u=0, i");
        request.set("Sec-Fetch-Dest", "document");
        request.set("Sec-Fetch-Mode", "navigate");
        request.set("Sec-Fetch-Site", "none");
        request.set("Sec-Fetch-User", "?1");
        request.set("Sec-GPC", "1");
        request.set("Upgrade-Insecure-Requests", "1");
        
        session->sendRequest(request);

        Poco::Net::HTTPResponse response;
        std::istream* responseStream = &session->receiveResponse(response);

        int redirectCount = 0;
        while ((response.getStatus() == 301 || response.getStatus() == 302) && redirectCount < 5) {
            std::string location = response.get("Location");
            Poco::URI newUri(location);
            return fetchJsonData(newUri.toString(), responseData);
            redirectCount++;
        }

        Poco::StreamCopier::copyStream(*responseStream, responseData);

        std::cerr << "HTTP Status: " << response.getStatus() << std::endl;
        //std::cerr << "Response body: " << responseData.str() << std::endl;

        return response;
    } catch (const Poco::Exception& e) {
        std::cerr << "HTTP request error: " << e.displayText() << std::endl;
        throw;
    }
}

inline void fetchAndDownloadJson(const std::string& url, sqlite3* db, const std::string& inputpath) {
    try {
        std::stringstream responseData;
        Poco::Net::HTTPResponse response = fetchJsonData(url, responseData);
        
        if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK) {
            std::cerr << "Failed to fetch JSON. HTTP Status: " << response.getStatus() << std::endl;
            return;
        }

        std::string body_data = responseData.str();
        if (body_data.empty()) {
            std::cerr << "Received empty response body!" << std::endl;
            return;
        }

        auto get_header = [](const Poco::Net::HTTPResponse& response, const std::string& header_name) -> std::string {
            try {
                return response.get(header_name);
            } catch (const Poco::NotFoundException&) {
                return "";
            }
        };

        auto decompress_if_needed = [&get_header](const Poco::Net::HTTPResponse& response, std::string& body_data) {
            auto encoding = get_header(response, "Content-Encoding");
            if (encoding == "gzip" || encoding == "deflate" || encoding == "br") {
                body_data = decompress_gzip(body_data);
            }
        };

        decompress_if_needed(response, body_data);

        auto last_modified = get_header(response, "Last-Modified");

        std::string saved_last_modified;
        bool has_saved_data = read_header(db, "Last-Modified", saved_last_modified);

        // Checking Last-Modified header ; trimws.h to handle white spaces.
auto is_modified = [&saved_last_modified, &last_modified](const std::string& header_value) -> bool {
    return saved_last_modified != trim(header_value) || last_modified != trim(header_value);
};
        std::string file_path = inputpath + "/company_tickers_exchange.json";
        manage_backups(file_path);

        // Check if the content has changed based on Last-Modified header
        if (!has_saved_data || is_modified(last_modified)) {
            save_json_to_file(body_data, file_path);
            save_header(db, "Last-Modified", last_modified);
            load_and_parse_json(file_path, db);
        } else {
            std::cout << "No change in Last-Modified header, skipping download." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in fetchAndDownloadJson: " << e.what() << std::endl;
    }
}

#endif
