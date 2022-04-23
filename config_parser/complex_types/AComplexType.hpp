#ifndef ACOMPLEX_TYPE_HPP
#define ACOMPLEX_TYPE_HPP

#include "../IParseable.hpp"
#include "Pattern.hpp"

class AComplexType: public IParseable
{
protected:
    rgx::Pattern pattern;

public:
    AComplexType(rgx::Pattern const &pattern);
    ~AComplexType();

    virtual int get_int();
    virtual double get_double();
    virtual std::string &get_string();
    virtual std::vector<std::string> &get_str_array();
    virtual std::vector<int> &get_int_array();
    virtual std::vector<double> &get_double_array();
    virtual IParseable &operator[](std::string);
};



#endif