#ifndef ASIMPLE_TYPE_HPP
#define ASIMPLE_TYPE_HPP

#include <stdexcept>

#include "../IParseable.hpp"
#include "Pattern.hpp"




class ASimpleType : public IParseable
{
protected:
    rgx::Pattern *pattern;
    virtual void    set_value(std::string const &raw) = 0;

public:
    ASimpleType(rgx::Pattern const &pattern);
    virtual ~ASimpleType();

    virtual int                         get_int();
    virtual double                      get_double();
    virtual std::string                 &get_string();
    virtual std::vector<std::string>    &get_str_array();
    virtual std::vector<int>            &get_int_array();
    virtual std::vector<double>         &get_double_array();
    virtual IParseable                  &operator[](std::string const &);
    virtual bool                        parse(std::string &str, size_t &idx);
};

#endif