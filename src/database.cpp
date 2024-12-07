#include "database.h"
#include "database_read.h"
#include "database_write.h"
#include "database_utility_handling.h"

#include <iostream>

#include <sqlite3.h>


void open_database(const std::string& full_db_path, sqlite3*& db) {
    open_db_connection(full_db_path, db); // database_read.cpp
}

void init_db(sqlite3*& db) {
    initialize_db_schema(db); // database_utility_handling.cpp
}

void insert_ticker_data(sqlite3* db, const std::vector<std::vector<std::string>>& ticker_data) {
    insert_data_into_tickers(db, ticker_data); // http_write.cpp
}

bool read_header_from_db(sqlite3* db, const std::string& header_name, std::string& header_value) {
    return read_header(db, header_name, header_value); // http_read.cpp
}

void save_headers_to_db(sqlite3* db, const std::string& header_name, const std::string& header_value) {
    save_header(db, header_name, header_value); // http_read.cpp
}
