#ifndef HTTP_H
#define HTTP_H

#include <string>

#include <sqlite3.h>
#include <cpprest/http_client.h>


web::http::http_response fetchJsonData(
const std::string& url
);

void fetchAndDownloadJson(
const std::string& url,
sqlite3* db,
const std::string& inputpath
);

#endif