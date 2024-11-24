// File Operations

#include "file_ops.h"

#include <fstream>
#include <iostream>

#include <sys/stat.h>
#include <nlohmann/json.hpp>

// Function to create directory if it doesn't exist
void create_directory(const std::string& dir) {
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) {
        if (mkdir(dir.c_str(), 0777) != 0) {
            std::cerr << "Error creating directory: " << dir << std::endl;
        }
    }
}

void manage_backups(const std::string& filename) {
    // Lambda to check if file exists
    auto file_exists = [](const std::string& filepath) {
        return std::ifstream(filepath).good();
    };

    std::string backup_filename = filename + "_backup";

    // Lambda to remove a file
    auto remove_file = [](const std::string& filepath) {
        std::remove(filepath.c_str());
    };

    // Lambda to rename a file
    auto rename_file = [](const std::string& old_name, const std::string& new_name) {
        std::rename(old_name.c_str(), new_name.c_str());
    };

    // If the original file exists, proceed with backup management
    if (file_exists(filename)) {
        if (file_exists(backup_filename)) {
            remove_file(backup_filename);  // Remove the old backup if it exists
        }
        rename_file(filename, backup_filename);  // Rename the original file to backup
    }
}