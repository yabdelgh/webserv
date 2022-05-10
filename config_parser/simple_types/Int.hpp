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
    Int(rgx::Pattern const &pattern, int default_val);
    Int(Int const &other);
    ~Int();
    Int & operator=(Int const &other);
    int get_int();
    IParseable *clone() const;
};

#endif