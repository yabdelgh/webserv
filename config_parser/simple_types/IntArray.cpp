#include "./IntArray.hpp"

IntArray::IntArray(rgx::Pattern const &pattern):ASimpleType(pattern) {}

IntArray::~IntArray() {}

void    IntArray::set_value(std::string const &raw) 
{
    // you may need to save the raw just in case you need the remainder.
    size_t pos = raw.find_first_of("0123456789", pos);
    while (pos != -1)
    {
        value.push_back(std::stoi(raw.substr(pos), &pos));
        pos = raw.find_first_of("0123456789", pos);
    }
}

std::vector<int> &IntArray::get_int_array() 
{
    return value;
}

