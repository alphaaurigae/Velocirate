#include "database_write.h"

#include <iostream>

#include <sqlite3.h>


void save_header(sqlite3* db, const std::string& header_name, const std::string& header_value) {
    const char* upsert_sql = 
        "INSERT INTO headers (header_name, header_value) "
        "VALUES (?, ?) "
        "ON CONFLICT(header_name) DO UPDATE SET header_value = ?;";
    
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, upsert_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare upsert statement!" << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, header_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, header_value.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, header_value.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to upsert data!" << std::endl;
    }

    sqlite3_finalize(stmt);
}

void insert_data_into_tickers(sqlite3* db, const std::vector<std::vector<std::string>>& ticker_data) {
    if (ticker_data.empty()) {
        std::cerr << "No ticker data to insert." << std::endl;
        return;
    }

    const char* insert_sql = 
        "INSERT INTO company_tickers (cik, name, ticker, exchange) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    char* err_msg = nullptr;

    if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::cerr << "Failed to begin transaction: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        return;
    }

    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement!" << std::endl;
        return;
    }

    auto insert_row = [&stmt](const std::vector<std::string>& row) {
        sqlite3_bind_text(stmt, 1, row[0].empty() ? "" : row[0].c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, row[1].empty() ? "" : row[1].c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, row[2].empty() ? "" : row[2].c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, row[3].empty() ? "" : row[3].c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to insert row: CIK: " << row[0] << " Name: " << row[1] << " Ticker: " << row[2] << " Exchange: " << row[3] << std::endl;
        }

        sqlite3_reset(stmt);
    };

    int rows_inserted = 0;
    for (const auto& row : ticker_data) {
        insert_row(row);
        rows_inserted++;
    }

    sqlite3_finalize(stmt);

    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::cerr << "Failed to commit transaction: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    } else {
        std::cout << rows_inserted << " rows successfully inserted." << std::endl;
    }
}