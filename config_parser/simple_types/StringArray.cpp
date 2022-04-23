#include "./StringArray.hpp"

StringArray::StringArray(rgx::Pattern const &pattern):ASimpleType(pattern) {}

StringArray::~StringArray() {}

void    StringArray::set_value(std::string const &raw) 
{
    // you may need to save the raw just in case you need the remainder.
    size_t end = 0;
    size_t start = raw.find_first_not_of(delimiter, end);

    while (start != -1)
    {
        size_t end = std::min(raw.find_first_of(delimiter, start), raw.size());
        value.push_back(raw.substr(start, end));
        size_t start = raw.find_first_not_of(delimiter, end);
    }
}

std::vector<std::string> &StringArray::get_str_array() 
{
    return value;
}

