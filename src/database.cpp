#include "database.h"
#include "database_read.h"
#include "database_write.h"
#include "database_utility_handling.h"

#include <iostream>

#include <sqlite3.h>

// Function to open SQLite database
void open_database(const std::string& full_db_path, sqlite3*& db) {
    // Logic for opening the database, now handled by database_read.cpp
    open_db_connection(full_db_path, db);
}

// Function to initialize the database schema
void init_db(sqlite3*& db) {
    // Initialize database schema, now handled by database_utility_handling.cpp
    initialize_db_schema(db);
}

// Function to insert ticker data into the database
void insert_ticker_data(sqlite3* db, const std::vector<std::vector<std::string>>& ticker_data) {
    // Insert ticker data, now handled by database_write.cpp
    insert_data_into_tickers(db, ticker_data);
}

// Function to read headers from the database
bool read_header_from_db(sqlite3* db, const std::string& header_name, std::string& header_value) {
    // Read header value, now handled by database_read.cpp
    return read_header(db, header_name, header_value);
}

// Function to save headers to the database
void save_headers_to_db(sqlite3* db, const std::string& header_name, const std::string& header_value) {
    // Save headers, now handled by database_write.cpp
    save_header(db, header_name, header_value);
}
