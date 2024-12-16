#ifndef HTTP_WRITE_H
#define HTTP_WRITE_H


#include "http_read.h"
#include "config.h"
#include "decompress.h"
#include "file_ops.h"
#include "database_read.h"
#include "database_write.h"
#include "init_sqlite_cik.h"
#include "trimws.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#include <cpprest/http_client.h>
#include <nlohmann/json.hpp>


inline void save_json_to_file(const std::string& body_data, const std::string& filename) {
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

inline void load_and_parse_json(const std::string& filepath, sqlite3* db) {
    auto open_file = [](const std::string& filepath) -> std::ifstream {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Failed to open JSON file: " << filepath << std::endl;
        }
        return file;
    };

    std::ifstream file = open_file(filepath);
    if (!file.is_open()) {
        return;
    }

    nlohmann::json json_data;
    try {
        file >> json_data;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse JSON file: " << e.what() << std::endl;
        return;
    }

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

    if (!validate_json(json_data)) {
        return;
    }

    const auto& data = json_data["data"];


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

    auto insert_data = [&ticker_data](const std::vector<std::string>& row) {
        ticker_data.push_back(row);
    };

    for (const auto& row : data) {
        insert_data(parse_row(row));
    }

    if (ticker_data.empty()) {
        std::cerr << "No valid ticker data parsed from JSON." << std::endl;
        return;
    }

    insert_data_into_tickers(db, ticker_data);
}

#endif