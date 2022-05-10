#include "./String.hpp"


String::String(rgx::Pattern const &pattern):ASimpleType(pattern) {}

String::String(rgx::Pattern const &pattern, std::string const &default_val):ASimpleType(pattern)
{
    value = default_val;
}

String::String(String const &other) {
    *this = other;
}

String::~String() {}

String &String::operator=(String const &other) 
{
    if (this != &other)
    {
        value = other.value;
        ASimpleType::operator=(other);
    }
    return *this;
}

void    String::set_value(std::string const &raw) 
{
    value = raw;
}

std::string &String::get_string()
{
    return value;
}

IParseable *String::clone() const
{
    return new String(*this);
}
