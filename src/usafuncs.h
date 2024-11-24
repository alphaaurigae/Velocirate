#ifndef USAFUNCS_H
#define USAFUNCS_H

#include <string>

void print_usage();  // Declaration of the usage function
void parse_arguments(int argc, char* argv[], std::string& inputpath, std::string& db_path);

#endif