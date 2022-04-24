#ifndef FREQUENT_HPP
#define FREQUENT_HPP

#include "./AComplexType.hpp"

class Frequent: public AComplexType
{

private:
    std::vector<IParseable*> parseables;
    IParseable* parseable;

public:
    Frequent(rgx::Pattern const &pattern);
    ~Frequent();
    virtual bool parse(std::string &str, size_t &idx);
    virtual IParseable &operator[](size_t idx);
};



#endif