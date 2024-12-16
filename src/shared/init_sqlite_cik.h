#ifndef INIT_SQLITE_CIK_H
#define INIT_SQLITE_CIK_H


#include <sqlite3.h>
#include <iostream>


inline void initialize_db_schema(sqlite3*& db) {
    const char* create_headers_table_sql = 
        "CREATE TABLE IF NOT EXISTS headers ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "header_name TEXT UNIQUE, "
        "header_value TEXT);";

    const char* create_tickers_table_sql = 
        "CREATE TABLE IF NOT EXISTS company_tickers ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "cik INTEGER, "
        "name TEXT, "
        "ticker TEXT, "
        "exchange TEXT);";

    auto execute_sql = [&db](const char* sql, const std::string& table_name) {
        char* err_msg = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
            std::cerr << "SQLite error (" << table_name << " table): " << err_msg << std::endl;
            sqlite3_free(err_msg);
        } else {
            std::cout << table_name << " table created/verified successfully." << std::endl;
        }
    };

    execute_sql(create_headers_table_sql, "headers");
    execute_sql(create_tickers_table_sql, "company_tickers");
}

#endif