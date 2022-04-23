#include "./ASimpleType.hpp"

ASimpleType::ASimpleType(rgx::Pattern const &pattern)
{
    this->pattern(pattern.clone());
}

ASimpleType::~ASimpleType()
{
    free(pattern);
}

int ASimpleType::get_int() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

double ASimpleType::get_double() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::string ASimpleType::&get_string() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<std::string> ASimpleType::&get_str_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<int> ASimpleType::&get_int_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<double> ASimpleType::&get_double_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

IParseable ASimpleType::&operator[](std::string) 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

bool ASimpleType::parse(std::string &str, size_t &idx)
{
    if (pattern.find(str, idx))
    {
        set_value(pattern.get);
        return true;
    }
    return false;
}