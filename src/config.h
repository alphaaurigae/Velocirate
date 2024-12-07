// Configuration variables
#ifndef CONFIG_H
#define CONFIG_H

#include <string>


// Files
const std::string DB_FILENAME = "data.db";
// const std::string JSON_FILENAME = "company_tickers_exchange.json"; 
// JSON filename main.cpp "std::string url = "https://www.sec.gov/files/" + JSON_FILENAME;" using plain url  so the above line is commented.

// Directories
const std::string DEFAULT_INPUT_PATH = "input";         // Default input path for saving the JSON file
const std::string DEFAULT_DB_PATH = "db";               // Default database path


// URL
const std::string JSON_URL = "https://www.sec.gov/files/company_tickers_exchange.json";

#endif