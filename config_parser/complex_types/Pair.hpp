#ifndef PAIR_HPP
#define PAIR_HPP

#include "./AComplexType.hpp"
#include <map>

class Pair: public AComplexType
{

private:
    std::map<std::string, IParseable*> parseables;
    rgx::Pattern key_ptrn;

public:
    Pair(IParseable const &parseable);
    Pair(Pair const &other);
    ~Pair();
    Pair &operator=(Pair const &other);
    bool parse(std::string &str, size_t &idx);
    IParseable &operator[](size_t idx);
    size_t size() const;
    IParseable *clone() const;
};



#endif