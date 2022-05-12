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
    size_t pos = 0;
    const char  * tmp = raw.c_str();
    do
    {
        tmp += strcspn(tmp, "-0123456789");
        value.push_back(std::stoi(tmp, &pos));
        tmp += pos;
    }while (*tmp);
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