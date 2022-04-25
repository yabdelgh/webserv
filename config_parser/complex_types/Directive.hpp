#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "./AComplexType.hpp"

class Directive: public AComplexType
{

private:
    std::vector<std::pair<std::string, IParseable*> > parseables;

public:
    Directive();
    Directive(Directive const &other);
    ~Directive();
    Directive &operator=(Directive const &other);
    Directive &push_parseable(std::string const &name, IParseable const& parseable);
    Directive &push_parseable(IParseable const& parseable);
    bool parse(std::string &str, size_t &idx);
    IParseable &operator[](std::string);
    IParseable &operator[](size_t idx);
    size_t size();
    IParseable *clone() const;
};



#endif