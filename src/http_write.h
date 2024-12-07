#ifndef HTTP_WRITE_H
#define HTTP_WRITE_H

#include <sqlite3.h>
#include <string>


void save_json_to_file(
const std::string& body_data, 
const std::string& filename
);


void load_and_parse_json(
const std::string& filepath,
sqlite3* db
);

#endif