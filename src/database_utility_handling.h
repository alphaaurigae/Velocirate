#ifndef DATABASE_UTILITY_HANDLING_H
#define DATABASE_UTILITY_HANDLING_H

#include <sqlite3.h>

void initialize_db_schema(
sqlite3*& db
);

#endif