#ifndef ACOMPLEX_TYPE_HPP
#define ACOMPLEX_TYPE_HPP

#include "../AParseable.hpp"
#include "Pattern.hpp"
#include <stdexcept>

class AComplexType: public AParseable
{
protected:
    AComplexType();

public:
    AComplexType(rgx::Pattern const &pattern);
    ~AComplexType();
};



#endif