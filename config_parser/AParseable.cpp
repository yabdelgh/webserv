#include "./AParseable.hpp"

AParseable::AParseable(/* args */) {}

AParseable::~AParseable() {}

int AParseable::get_int()
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

double AParseable::get_double()
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::string &AParseable::get_string() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<std::string> &AParseable::get_str_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<int> &AParseable::get_int_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::vector<double> &AParseable::get_double_array() 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

IParseable &AParseable::operator[](std::string) 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

IParseable &AParseable::operator[](size_t idx) 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

size_t AParseable::size() const
{
    return 0;
}