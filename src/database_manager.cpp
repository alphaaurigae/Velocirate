#include "database_manager.h"

#include <iostream>

DatabaseManager::DatabaseManager(const std::string& db_path)
    : db(nullptr, sqlite3_close) {
    if (!openDatabase(db_path)) {
        std::cerr << "Failed to open database: " << db_path << std::endl;
    }
}

DatabaseManager::~DatabaseManager() = default;

bool DatabaseManager::openDatabase(const std::string& db_path) {
    sqlite3* raw_db = nullptr;
    if (sqlite3_open(db_path.c_str(), &raw_db) != SQLITE_OK) {
        return false;
    }
    db.reset(raw_db);
    return true;
}

bool DatabaseManager::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db.get(), query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

sqlite3_stmt* DatabaseManager::prepareStatement(const std::string& query) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db.get(), query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    return stmt;
}

void DatabaseManager::finalizeStatement(sqlite3_stmt* stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}