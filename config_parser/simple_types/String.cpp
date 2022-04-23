#include "./String.hpp"


String::String(rgx::Pattern const &pattern):ASimpleType(pattern) {}

String::~String() {}

void    String::set_value(std::string const &raw) 
{
    value = raw;
}

std::string &String::get_string()
{
    return value;
}

