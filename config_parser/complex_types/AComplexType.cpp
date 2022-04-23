#include "./AComplexType.hpp"

AComplexType::AComplexType(rgx::Pattern const &pattern):pattern(pattern) {}

AComplexType::~AComplexType() {}

int AComplexType::get_int()
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

double AComplexType::get_double()
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::string &AComplexType::get_string() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<std::string> &AComplexType::get_str_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<int> &AComplexType::get_int_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<double> &AComplexType::get_double_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

IParseable &AComplexType::operator[](std::string) 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}