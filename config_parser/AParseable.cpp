#include "./AParseable.hpp"

AParseable::AParseable(/* args */)
{
    parsed = false;
    reached_end = false;
}

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

IParseable &AParseable::operator[](std::string const &) 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

IParseable &AParseable::operator[](size_t idx) 
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

size_t AParseable::size() const
{
    return -1;
}

bool AParseable::cont_parse(std::string &str, size_t &idx)
{
    return parse(str, idx);
}

bool AParseable::is_reached_end() const 
{
    return reached_end;
}

bool AParseable::is_parsed() const
{
    return parsed;
}

bool AParseable::contains(std::string const &name)
{
    try {
        (*this)[name];
        return true;
    }catch (const std::exception)
    {
        return false;
    }
}

std::string & AParseable::str() { return this->get_string(); }
std::vector<std::string> & AParseable::str_arr() { return get_str_array(); }
std::vector<int> & AParseable::int_arr() { return get_int_array(); }
std::vector<double> & AParseable::dbl_arr() { return get_double_array(); }
int AParseable::num() { return get_int(); }
double AParseable::dbl() { return get_double(); }

std::set<int> &AParseable::int_set()
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::set<std::string> &AParseable::str_set()
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

std::set<double> &AParseable::dbl_set()
{
    throw std::runtime_error(INVALID_TYPE_ERROR);
}

bool AParseable::parse(std::string const &str, size_t &idx)
{
    parsed = parseme(str, idx);
    return parsed;
}

bool AParseable::parse(std::vector<char> str, size_t &idx)
{
    std::string s(str.data(), str.size());
    return parse(s, idx);
}