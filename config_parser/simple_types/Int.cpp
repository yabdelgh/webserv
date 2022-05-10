#include "./Int.hpp"


Int::Int(rgx::Pattern const &pattern):ASimpleType(pattern) {}

Int::Int(rgx::Pattern const &pattern, int default_val):ASimpleType(pattern)
{
    value = default_val;
}

Int::~Int() {}

Int::Int(Int const &other)
{
    *this = other;
}

Int &Int::operator=(Int const &other)
{
    if (this != &other) 
    {
        value = other.value;
        ASimpleType::operator=(other);
    }
    return *this;
}

IParseable *Int::clone() const
{
    return new Int(*this);
}

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

