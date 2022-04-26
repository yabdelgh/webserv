#ifndef IPARSEABLE_HPP
#define IPARSEABLE_HPP

#include<string>
#include<vector>

#define INVALID_TYPE_ERROR "INVALID TYPE"

class IParseable
{
private:
    /* data */
public:
    virtual int get_int() = 0;
    virtual double get_double() = 0;
    virtual std::string &get_string() = 0;
    virtual std::vector<std::string> &get_str_array() = 0;
    virtual std::vector<int> &get_int_array() = 0;
    virtual std::vector<double> &get_double_array() = 0;
    virtual IParseable &operator[](std::string) = 0;
    virtual IParseable &operator[](size_t idx) = 0;
    virtual size_t size() const = 0;
    virtual IParseable *clone() const = 0;
    virtual bool parse(std::string &str, size_t &idx) = 0;
    virtual ~IParseable() {};
};

#endif