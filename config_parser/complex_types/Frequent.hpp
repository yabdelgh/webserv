#ifndef FREQUENT_HPP
#define FREQUENT_HPP

#include "./AComplexType.hpp"

class Frequent: public AComplexType
{

private:
    std::vector<IParseable*> parseables;
    IParseable* parseable;

public:
    Frequent(IParseable const &parseable);
    Frequent(Frequent const &other);
    ~Frequent();
    Frequent &operator=(Frequent const &other);
    bool parse(std::string &str, size_t &idx);
    IParseable &operator[](size_t idx);
    size_t size();
    IParseable *clone() const;
};



#endif