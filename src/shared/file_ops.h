#ifndef FILE_OPS_H
#define FILE_OPS_H


#include <fstream>
#include <iostream>
#include <string>

#include <sys/stat.h>
#include <nlohmann/json.hpp>


inline void create_directory(const std::string& dir) {
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) {
        if (mkdir(dir.c_str(), 0777) != 0) {
            std::cerr << "Error creating directory: " << dir << std::endl;
        }
    }
}

inline void manage_backups(const std::string& filename) {

    auto file_exists = [](const std::string& filepath) {
        return std::ifstream(filepath).good();
    };

    std::string backup_filename = filename + "_backup";


    auto remove_file = [](const std::string& filepath) {
        std::remove(filepath.c_str());
    };


    auto rename_file = [](const std::string& old_name, const std::string& new_name) {
        std::rename(old_name.c_str(), new_name.c_str());
    };


    if (file_exists(filename)) {
        if (file_exists(backup_filename)) {
            remove_file(backup_filename);
        }
        rename_file(filename, backup_filename);
    }
}

#endif