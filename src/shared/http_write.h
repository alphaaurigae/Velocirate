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

// hack to use poco format ...  ensure that Poco::format is used, isntead of std::format. e.g Poco::Timespan && Poco::DateTim may work instead anyways.
/*#define POCO_NO_CXX20_FORMAT
#include <Poco/Format.h>
#include <chrono>*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>


#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/FileStream.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>

inline void save_json_to_file(const std::string& body_data, const std::string& filename) {
    try {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(body_data);
        Poco::JSON::Object::Ptr json_data = result.extract<Poco::JSON::Object::Ptr>();

        std::ofstream file(filename);
        if (file.is_open()) {
            Poco::JSON::Stringifier::stringify(json_data, file, 4);
            file.close();
            std::cout << "File saved to: " << filename << std::endl;
        } else {
            std::cerr << "Error opening file for saving: " << filename << std::endl;
        }
    } catch (const Poco::JSON::JSONException& e) {
        std::cerr << "Failed to parse JSON: " << e.displayText() << std::endl;
    }
}

inline void load_and_parse_json(const std::string& filepath, sqlite3* db) {
    auto open_file = [](const std::string& filepath) -> std::unique_ptr<Poco::FileInputStream> {
        auto file = std::make_unique<Poco::FileInputStream>(filepath);
        if (!file) {
            std::cerr << "Failed to open JSON file: " << filepath << std::endl;
            return nullptr;
        }
        return file;
    };

    auto file = open_file(filepath);
    if (!file) {
        return;
    }

    Poco::JSON::Parser parser;
    Poco::Dynamic::Var parsed_result;

    try {
        parsed_result = parser.parse(*file);
    } catch (const Poco::JSON::JSONException& e) {
        std::cerr << "Failed to parse JSON file: " << e.displayText() << std::endl;
        return;
    }

    Poco::JSON::Object::Ptr json_data = parsed_result.extract<Poco::JSON::Object::Ptr>();
    if (!json_data->has("fields") || !json_data->has("data")) {
        std::cerr << "JSON does not contain expected 'fields' or 'data' keys." << std::endl;
        return;
    }

    Poco::JSON::Array::Ptr fields = json_data->getArray("fields");
    if (fields->size() != 4 || fields->getElement<std::string>(0) != "cik" ||
        fields->getElement<std::string>(1) != "name" ||
        fields->getElement<std::string>(2) != "ticker" ||
        fields->getElement<std::string>(3) != "exchange") {
        std::cerr << "Unexpected fields format in JSON." << std::endl;
        return;
    }

    Poco::JSON::Array::Ptr data = json_data->getArray("data");
    if (!data) {
        std::cerr << "No valid 'data' array in the provided JSON." << std::endl;
        return;
    }

    auto parse_row = [](const Poco::JSON::Array::Ptr& row) {
        auto parse_field = [](const Poco::Dynamic::Var& field, bool is_string = true) -> std::string {
            if (field.isEmpty()) return "";
            if (is_string && field.isString()) return field.convert<std::string>();
            if (!is_string && field.isInteger()) return std::to_string(field.convert<int>());
            return "";
        };

        return std::vector<std::string>{
            parse_field(row->get(0), false), // CIK as integer
            parse_field(row->get(1)),         // Name as string
            parse_field(row->get(2)),         // Ticker as string
            parse_field(row->get(3))          // Exchange as string
        };
    };

    std::vector<std::vector<std::string>> ticker_data;

    for (size_t i = 0; i < data->size(); ++i) {
        Poco::JSON::Array::Ptr row = data->getArray(i);
        if (row) {
            ticker_data.push_back(parse_row(row));
        }
    }

    if (ticker_data.empty()) {
        std::cerr << "No valid ticker data parsed from JSON." << std::endl;
        return;
    }

    insert_data_into_tickers(db, ticker_data);
}




#endif