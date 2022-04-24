#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>

# define WHITESPACE " \n\r\t\f\v"

// trims
std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

#endif