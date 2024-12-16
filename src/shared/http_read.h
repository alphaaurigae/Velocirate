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

#include <cpprest/http_client.h>
#include <nlohmann/json.hpp>


// Function to fetch JSON data from a URL
inline web::http::http_response fetchJsonData(const std::string& url) {
    try {
        web::http::client::http_client client(U(url));
        web::http::http_request request(web::http::methods::GET);
        request.headers().add(U("Accept"), U("application/json"));
        request.headers().add(U("Accept-Encoding"), U("gzip, deflate, br"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0"));
        return client.request(request).get();
    } catch (const std::exception& e) {
        std::cerr << "HTTP request error: " << e.what() << std::endl;
        throw;
    }
}

// Function to fetch JSON from a URL and download it to a file, then process it
inline void fetchAndDownloadJson(const std::string& url, sqlite3* db, const std::string& inputpath) {
    try {
        web::http::http_response response = fetchJsonData(url);
        if (response.status_code() != web::http::status_codes::OK) {
            std::cerr << "Failed to fetch JSON. HTTP Status: " << response.status_code() << std::endl;
            return;
        }

        std::string body_data = response.extract_utf8string().get();
        if (body_data.empty()) {
            std::cerr << "Received empty response body!" << std::endl;
            return;
        }

        auto get_header = [](const web::http::http_headers& headers, const std::string& header_name) -> std::string {
            auto it = headers.find(header_name);
            return (it != headers.end()) ? it->second : "";
        };

        auto decompress_if_needed = [&get_header](const web::http::http_headers& headers, std::string& body_data) {
            auto encoding = get_header(headers, "Content-Encoding");
            if (encoding == "gzip" || encoding == "deflate" || encoding == "br") {
                body_data = decompress_gzip(body_data);
            }
        };

        decompress_if_needed(response.headers(), body_data);

        auto last_modified = get_header(response.headers(), "Last-Modified");

        std::string saved_last_modified;
        bool has_saved_data = read_header(db, "Last-Modified", saved_last_modified);

        // Checking Last-Modified header
        auto is_modified = [&saved_last_modified, &last_modified](const std::string& header_value) -> bool {
            return saved_last_modified != trim(header_value);
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