#include "database_read.h"

#include <sqlite3.h>

#include <iostream>

void open_db_connection(const std::string& full_db_path, sqlite3*& db) {
    if (sqlite3_open(full_db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        if (db) sqlite3_close(db); 
        std::exit(1);
    }
    std::cout << "Database opened successfully!" << std::endl;
}

bool read_header(sqlite3* db, const std::string& header_name, std::string& header_value) {
    const char* select_sql = "SELECT header_value FROM headers WHERE header_name = ? LIMIT 1;";
    
    // Lambda to prepare, bind, and finalize the statement
    auto execute_sql = [&db, select_sql](const std::string& param, std::string& result_value) -> bool {
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare select statement!" << std::endl;
            return false;
        }

        sqlite3_bind_text(stmt, 1, param.c_str(), -1, SQLITE_STATIC);
        
        bool success = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            success = true;
        }

        sqlite3_finalize(stmt);
        return success;
    };

    // Use the lambda to execute the query
    return execute_sql(header_name, header_value);
}