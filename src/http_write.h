#ifndef HTTP_WRITE_H
#define HTTP_WRITE_H

#include <sqlite3.h>
#include <string>

// Save JSON data to a file
void save_json_to_file(const std::string& body_data, const std::string& filename);

// Load and parse the saved JSON file and insert data into the database
void load_and_parse_json(const std::string& filepath, sqlite3* db);

#endif