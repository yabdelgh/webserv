#ifndef APARSEABLE_HPP
#define APARSEABLE_HPP

#include "./IParseable.hpp"
#include <stdexcept>

class AParseable : public IParseable
{
protected:
    bool reached_end;
public:
    AParseable(/* args */);
    virtual ~AParseable();

    virtual int get_int();
    virtual double get_double();
    virtual std::string &get_string();
    virtual std::vector<std::string> &get_str_array();
    virtual std::vector<int> &get_int_array();
    virtual std::vector<double> &get_double_array();
    virtual IParseable &operator[](std::string const &);
    virtual IParseable &operator[](size_t idx);
    virtual bool cont_parse(std::string &str, size_t &idx);
    virtual size_t size() const;
    bool is_reached_end() const;
};

#endif