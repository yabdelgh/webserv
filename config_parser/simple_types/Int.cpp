#include "./Int.hpp"


Int::Int(rgx::Pattern const &pattern):ASimpleType(pattern) {}

Int::~Int() {}

void    Int::set_value(std::string const &raw) 
{
    // you may need to save the raw just in case you need the remainder.
    size_t pos = raw.find_first_of("0123456789");
    value = std::stoi(raw.substr(pos));
}

int     Int::get_int() 
{
    return value;
}

