#ifndef DATABASE_WRITE_H
#define DATABASE_WRITE_H

#include <sqlite3.h>

#include <string>
#include <vector>

void save_header(sqlite3* db, const std::string& header_name, const std::string& header_value);
void insert_data_into_tickers(sqlite3* db, const std::vector<std::vector<std::string>>& ticker_data);

#endif