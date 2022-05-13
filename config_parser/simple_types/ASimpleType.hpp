#ifndef ASIMPLE_TYPE_HPP
#define ASIMPLE_TYPE_HPP

#include <stdexcept>

#include "../AParseable.hpp"
#include "Pattern.hpp"

class ASimpleType : public AParseable
{
protected:
    rgx::Pattern pattern;

protected:
    virtual void    set_value(std::string const &raw) = 0;
    ASimpleType();

public:
    ASimpleType(rgx::Pattern const &pattern);
    ASimpleType(ASimpleType const &other);
    virtual ~ASimpleType();

    ASimpleType &operator=(ASimpleType const &other);
    virtual bool parseme(std::string const &str, size_t &idx);
};

#endif