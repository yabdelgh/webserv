#ifndef INT_HPP
#define INT_HPP

#include "./ASimpleType.hpp"

class Int: public ASimpleType
{
private:
    int value;

protected:
    void set_value(std::string const &raw);

public:
    Int(rgx::Pattern const &pattern);
    ~Int();
    int get_int();
};

#endif