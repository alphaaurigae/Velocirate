#include "http.h"
#include "http_read.h"
#include "http_write.h"
#include "config.h"
#include "decompress.h"
#include "file_ops.h"
#include "database.h"
#include "utils.h"

#include <thread>
#include <fstream>
#include <chrono>
#include <iostream>

#include <cpprest/http_client.h>
#include <nlohmann/json.hpp>


void executeJsonFetch(const std::string& url, sqlite3* db, const std::string& inputpath) {
    fetchAndDownloadJson(url, db, inputpath);
}

//void save_json_to_file(const std::string& body_data, const std::string& filename) {
  //  ::save_json_to_file(body_data, filename);  // Correctly refer to the function in http_write.cpp
//}

//void load_and_parse_json(const std::string& filepath, sqlite3* db) {
  //  ::load_and_parse_json(filepath, db);  // Correctly refer to the function in http_write.cpp
//}