#include "database_read.h"
#include "database_write.h"
//#include "init_sqlite_cik.h"
#include "config.h"
#include "http_read.h"
#include "argument_parser.h"

#include <iostream>

#include <sqlite3.h>

int main(int argc, char* argv[]) {
    sqlite3* db = nullptr;
    std::string inputpath = DEFAULT_INPUT_PATH; // config.h
    std::string db_path = DEFAULT_DB_PATH; // config.h


    parse_arguments(argc, argv, inputpath, db_path);

    std::string full_db_path = db_path + "/" + DB_FILENAME;

    open_db_connection(full_db_path, db);

    initialize_db_schema(db);

    std::string url = JSON_URL; // config.h

    fetchAndDownloadJson(url, db, inputpath); // http_read.cpp

    if (db) sqlite3_close(db);
    return 0;
}