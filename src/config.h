// Configuration variables
#ifndef CONFIG_H
#define CONFIG_H

#include <string>


// Files
const std::string DB_FILENAME = "data.db";          // Centralized database filename
// const std::string JSON_FILENAME = "company_tickers_exchange.json"; // Centralized JSON filename main.cpp "std::string url = "https://www.sec.gov/files/" + JSON_FILENAME;" using plain url  so this is disabled.

// Directories
const std::string DEFAULT_INPUT_PATH = "input";         // Default input path for saving the JSON file
const std::string DEFAULT_DB_PATH = "db";               // Default database path


// URL
const std::string JSON_URL = "https://www.sec.gov/files/company_tickers_exchange.json"; // Centralized JSON URL

#endif