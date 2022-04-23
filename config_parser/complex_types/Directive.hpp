#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "./AComplexType.hpp"

class Directive: public AComplexType
{

private:
    std::vector<std::pair<std::string, IParseable*> > parseables;

public:
    Directive(rgx::Pattern const &pattern);
    ~Directive();
    void push_parseable(std::string const &name, IParseable const& parseable);
    void push_parseable(IParseable const& parseable);
    virtual bool parse(std::string &str, size_t &idx);
    virtual IParseable &operator[](std::string);
    virtual IParseable &operator[](size_t idx);
};



#endif