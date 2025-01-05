#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <iostream>
#include <string>

#include <Poco/File.h>
#include <Poco/Path.h>

inline void create_directory(const std::string& dir) {
    Poco::File dirFile(dir);
    if (!dirFile.exists()) {
        dirFile.createDirectories();
    }
}

inline void manage_backups(const std::string& filename) {
    Poco::File file(filename);
    Poco::File backupFile(filename + "_backup");

    if (file.exists()) {
        if (backupFile.exists()) {
            backupFile.remove();
        }
        file.renameTo(backupFile.path());
    }
}

#endif