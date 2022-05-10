#include "./IntArray.hpp"

IntArray::IntArray(rgx::Pattern const &pattern):ASimpleType(pattern) {}

IntArray::IntArray(rgx::Pattern const &pattern, std::vector<int> const& default_val)
{
    value = default_val;
}

IntArray::IntArray(IntArray const &other)
{
    *this = other;
}

IntArray::~IntArray() {}

IntArray &IntArray::operator=(IntArray const &other)
{
    if (this != &other)
    {
        this->value.clear();
        this->value = other.value;
        ASimpleType::operator=(other);
    }
    return *this;
}

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

IParseable *IntArray::clone() const 
{
    return new IntArray(*this);
}

std::set<int> &IntArray::int_set()
{
    if (set.size() != value.size())
        set.insert(value.begin(), value.end());
    return set;
}