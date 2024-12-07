#include "database.h"
#include "config.h"
#include "http.h"
#include "usafuncs.h"

#include <sqlite3.h>

#include <iostream>

int main(int argc, char* argv[]) {
    sqlite3* db = nullptr;
    std::string inputpath = DEFAULT_INPUT_PATH; // config.h
    std::string db_path = DEFAULT_DB_PATH; // config.h


    parse_arguments(argc, argv, inputpath, db_path);

    std::string full_db_path = db_path + "/" + DB_FILENAME;

    open_database(full_db_path, db); // database_read.cpp

    init_db(db);

    std::string url = JSON_URL; // config.h

    fetchAndDownloadJson(url, db, inputpath); // http_read.cpp

    if (db) sqlite3_close(db);
    return 0;
}