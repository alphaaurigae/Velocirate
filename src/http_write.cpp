#include "http.h"
#include "http_read.h"
#include "http_write.h"
#include "config.h"
#include "decompress.h"
#include "file_ops.h"
#include "database.h"
#include "utils.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#include <cpprest/http_client.h>
#include <nlohmann/json.hpp>


// Save JSON data to a file
void save_json_to_file(const std::string& body_data, const std::string& filename) {
    try {
        nlohmann::json json_data = nlohmann::json::parse(body_data);
        std::ofstream file(filename);
        if (file.is_open()) {
            file << json_data.dump(4);
            file.close();
            std::cout << "File saved to: " << filename << std::endl;
        } else {
            std::cerr << "Error opening file for saving: " << filename << std::endl;
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
    }
}


void load_and_parse_json(const std::string& filepath, sqlite3* db) {
    // Lambda for opening a file
    auto open_file = [](const std::string& filepath) -> std::ifstream {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open JSON file: " << filepath << std::endl;
        }
        return file;
    };

    // Open the file
    std::ifstream file = open_file(filepath);
    if (!file.is_open()) {
        return; // File opening failed, no need to continue
    }

    nlohmann::json json_data;
    try {
        file >> json_data;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse JSON file: " << e.what() << std::endl;
        return;
    }

    // Lambda for validating JSON
    auto validate_json = [](const nlohmann::json& json_data) -> bool {
        if (!json_data.contains("fields") || !json_data.contains("data")) {
            std::cerr << "JSON does not contain expected 'fields' or 'data' keys." << std::endl;
            return false;
        }

        const auto& fields = json_data["fields"];
        if (fields.size() != 4 || fields[0] != "cik" || fields[1] != "name" ||
            fields[2] != "ticker" || fields[3] != "exchange") {
            std::cerr << "Unexpected fields format in JSON." << std::endl;
            return false;
        }
        return true;
    };

    // Use the lambda for validation
    if (!validate_json(json_data)) {
        return;
    }

    const auto& data = json_data["data"];

    // Lambda function for parsing each row
    auto parse_row = [](const nlohmann::json& row) {
        auto parse_field = [](const nlohmann::json& field, bool is_string = true) -> std::string {
            if (field.is_null()) return "";
            if (is_string && field.is_string()) return field.get<std::string>();
            if (!is_string && field.is_number_integer()) return std::to_string(field.get<int>());
            return "";
        };

        return std::vector<std::string>{
            parse_field(row[0], false), // CIK as integer
            parse_field(row[1]),         // Name as string
            parse_field(row[2]),         // Ticker as string
            parse_field(row[3])          // Exchange as string
        };
    };

    std::vector<std::vector<std::string>> ticker_data;

    // Lambda for inserting data
    auto insert_data = [&ticker_data](const std::vector<std::string>& row) {
        ticker_data.push_back(row);
    };

    // Iterate over the data and use the lambda to parse and insert each row
    for (const auto& row : data) {
        insert_data(parse_row(row));
    }

    if (ticker_data.empty()) {
        std::cerr << "No valid ticker data parsed from JSON." << std::endl;
        return;
    }

    insert_ticker_data(db, ticker_data);
}