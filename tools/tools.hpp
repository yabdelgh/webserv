#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include "IParseable.hpp"
#include <cstring>


# define WHITESPACE " \n\r\t\f\v"

// trims
std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

// read file contents
std::string get_file_contents(std::string const& filename, std::string &);

IParseable *get_server_config();
IParseable *get_request_header();

std::string extension(std::string const &file_name);
std::string joinpath(std::string const &path1 , std::string const &path2);
std::string cleanpath(std::string const &path);

std::vector<std::string> split(std::string const & str, std::string const &delm);

#endif