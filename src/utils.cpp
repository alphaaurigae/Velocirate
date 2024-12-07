// Utility Functions

#include "utils.h"

#include <algorithm>
#include <cctype>

std::string trim(const std::string& str) {

    auto find_first_non_whitespace = [](const std::string& s) {
        return s.find_first_not_of(" \t\n\r\f\v");
    };

    auto find_last_non_whitespace = [](const std::string& s) {
        return s.find_last_not_of(" \t\n\r\f\v");
    };

    size_t first = find_first_non_whitespace(str);
    size_t last = find_last_non_whitespace(str);

    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}