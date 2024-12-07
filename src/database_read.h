#ifndef DATABASE_READ_H
#define DATABASE_READ_H

#include <string>

#include <sqlite3.h>

void open_db_connection(
const std::string& full_db_path,
sqlite3*& db
);

bool read_header(
sqlite3* db,
const std::string& header_name,
std::string& header_value
);

#endif