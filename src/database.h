#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

#include <string>
#include <vector>

// Function declarations
void init_db(sqlite3*& db);
bool read_header_from_db(sqlite3* db, const std::string& header_name, std::string& header_value);
void save_headers_to_db(sqlite3* db, const std::string& header_name, const std::string& header_value);

// Declare the open_database function
void open_database(const std::string& full_db_path, sqlite3*& db);

// Add this line to declare the insert_ticker_data function
void insert_ticker_data(sqlite3* db, const std::vector<std::vector<std::string>>& ticker_data);

#endif