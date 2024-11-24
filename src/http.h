#ifndef HTTP_H
#define HTTP_H

#include <string>

#include <sqlite3.h>
#include <cpprest/http_client.h>

// Fetch JSON data from the given URL
web::http::http_response fetchJsonData(const std::string& url);

// Function to fetch, download, save, and parse JSON data
void fetchAndDownloadJson(const std::string& url, sqlite3* db, const std::string& inputpath);

#endif // HTTP_H