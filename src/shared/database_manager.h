#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H


#include <sqlite3.h>
#include <iostream>
#include <memory>
#include <string>


class DatabaseManager {
public:
    explicit DatabaseManager(const std::string& db_path);
    ~DatabaseManager();

    bool openDatabase(const std::string& db_path);
    bool executeQuery(const std::string& query);
    sqlite3_stmt* prepareStatement(const std::string& query);
    void finalizeStatement(sqlite3_stmt* stmt);

private:
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> db;
};

// Constructor: Initializes and opens the database connection.
inline DatabaseManager::DatabaseManager(const std::string& db_path)
    : db(nullptr, sqlite3_close) {
    if (!openDatabase(db_path)) {
        std::cerr << "Failed to open database: " << db_path << std::endl;
    }
}

// Destructor: Default destructor (no need for custom implementation as we are using unique_ptr).
inline DatabaseManager::~DatabaseManager() = default;

// Opens the SQLite database.
inline bool DatabaseManager::openDatabase(const std::string& db_path) {
    sqlite3* raw_db = nullptr;
    if (sqlite3_open(db_path.c_str(), &raw_db) != SQLITE_OK) {
        return false;
    }
    db.reset(raw_db);
    return true;
}

// Executes a query on the database (no results expected).
inline bool DatabaseManager::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db.get(), query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Prepares an SQL statement for execution.
inline sqlite3_stmt* DatabaseManager::prepareStatement(const std::string& query) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db.get(), query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return nullptr;
    }
    return stmt;
}

// Finalizes a prepared statement.
inline void DatabaseManager::finalizeStatement(sqlite3_stmt* stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

#endif