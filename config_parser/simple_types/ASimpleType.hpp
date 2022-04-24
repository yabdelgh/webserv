#ifndef ASIMPLE_TYPE_HPP
#define ASIMPLE_TYPE_HPP

#include <stdexcept>

#include "../IParseable.hpp"
#include "Pattern.hpp"

class ASimpleType : public IParseable
{
protected:
    rgx::Pattern pattern;

protected:
    virtual void    set_value(std::string const &raw) = 0;

public:
    ASimpleType(rgx::Pattern const &pattern);
    virtual ~ASimpleType();
    virtual bool                        parse(std::string &str, size_t &idx);
};

#endif