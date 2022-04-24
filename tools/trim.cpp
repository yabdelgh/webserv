#include "tools.hpp"

std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    size_t end = s.find_last_not_of(WHITESPACE);
    if (start == -1 || end == -1)
        return "";
    return s.substr(start, end);
}
