#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <memory>
#include <string>

#include <sqlite3.h>

class DatabaseManager {
public:
    explicit DatabaseManager(const std::string& db_path);
    ~DatabaseManager();

    bool executeQuery(const std::string& query);
    sqlite3_stmt* prepareStatement(const std::string& query);
    void finalizeStatement(sqlite3_stmt* stmt);

private:
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> db;
    bool openDatabase(const std::string& db_path);
};

#endif // DATABASE_MANAGER_H