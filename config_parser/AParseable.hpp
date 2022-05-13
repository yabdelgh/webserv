#ifndef APARSEABLE_HPP
#define APARSEABLE_HPP

#include "./IParseable.hpp"
#include <stdexcept>

class AParseable : public IParseable
{
protected:
    bool parsed;
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

    virtual std::string &str();
    virtual std::vector<std::string> &str_arr();
    virtual std::vector<int> &int_arr();
    virtual std::vector<double> &dbl_arr();
    virtual int num();
    virtual double dbl();

    virtual std::set<int> &int_set();
    virtual std::set<std::string> &str_set();
    virtual std::set<double> &dbl_set();

    virtual IParseable &operator[](std::string const &);
    virtual IParseable &operator[](size_t idx);
    virtual bool cont_parse(std::string &str, size_t &idx);
    virtual size_t size() const;
    virtual bool contains(std::string const &name);
    virtual bool parseme(std::string const &str, size_t &idx) = 0;
    bool is_reached_end() const;
    bool is_parsed() const;
    bool parse(std::string const &str, size_t &idx);
};

#endif