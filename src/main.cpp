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

    // Parse command-line arguments
    parse_arguments(argc, argv, inputpath, db_path);

    // Construct full database path using db_path and DB_FILENAME
    std::string full_db_path = db_path + "/" + DB_FILENAME;

    // Call the function to open the database; it will handle the rest (including closing)
    open_database(full_db_path, db);

    init_db(db);

    // Use the centralized URL for fetching JSON
    std::string url = JSON_URL;

    // Pass the input path to the fetchAndDownloadJson function
    fetchAndDownloadJson(url, db, inputpath);

    if (db) sqlite3_close(db);
    return 0;
}