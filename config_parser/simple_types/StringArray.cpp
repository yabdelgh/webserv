#include "./StringArray.hpp"
#include "tools.hpp"

StringArray::StringArray(rgx::Pattern const &pattern, std::string delimiter):ASimpleType(pattern)
{
    this->delimiter = delimiter;
}

StringArray::StringArray(rgx::Pattern const &pattern, std::string *default_val,size_t size, std::string delimiter)
{
    this->delimiter = delimiter;
    value.assign(default_val, default_val + size);
}


StringArray::StringArray(StringArray const &other)
{
    *this = other;
}

StringArray::~StringArray() {}

StringArray &StringArray::operator=(StringArray const &other)
{
    if (this != &other)
    {
        this->set.clear();
        this->value.clear();
        this->value = other.value;
        this->delimiter = other.delimiter;
        ASimpleType::operator=(other);
    }
    return *this;
}

void    StringArray::set_value(std::string const &raw) 
{
    // you may need to save the raw just in case you need the remainder.
    // size_t end = 0;
    // size_t start = raw.find_first_not_of(delimiter, end);

    // while (start != -1)
    // {
    //     size_t end = std::min(raw.find_first_of(delimiter, start), raw.size());
    //     value.push_back(raw.substr(start, end));
    //     size_t start = raw.find_first_not_of(delimiter, end);
    // }
    value = split(raw, delimiter);
}

std::vector<std::string> &StringArray::get_str_array() 
{
    return value;
}

IParseable *StringArray::clone() const
{
    return new StringArray(*this);
}

std::set<std::string> &StringArray::str_set()
{
    if (set.size() != value.size())
        set.insert(value.begin(), value.end());
    return set;
}