#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H


#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>


inline void print_usage() {
    std::cout << "Usage: \n";
    std::cout << "  --inputpath, -ip <path>  Specify the input path for saving the JSON file\n";
    std::cout << "  --database, -db <path>   Specify the database directory path\n";
}

inline void parse_arguments(int argc, char* argv[], std::string& inputpath, std::string& db_path) {

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--inputpath") == 0 || std::strcmp(argv[i], "-ip") == 0) {
            if (i + 1 < argc) {
                inputpath = argv[++i];
            } else {
                std::cerr << "Error: Missing value for inputpath\n";
                print_usage();
                exit(1);
            }
        } else if (std::strcmp(argv[i], "--database") == 0 || std::strcmp(argv[i], "-db") == 0) {
            if (i + 1 < argc) {
                db_path = argv[++i];
            } else {
                std::cerr << "Error: Missing value for database\n";
                print_usage();
                exit(1);
            }
        } else {
            std::cerr << "Error: Unknown option " << argv[i] << "\n";
            print_usage();
            exit(1);
        }
    }
}

#endif