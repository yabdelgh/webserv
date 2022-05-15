#ifndef DIRECTIVE_HPP
#define DIRECTIVE_HPP

#include "./AComplexType.hpp"

class Directive: public AComplexType
{

private:
    std::vector<std::pair<std::string, IParseable*> > parseables;
    size_t last_index;

public:
    Directive();
    Directive(Directive const &other);
    ~Directive();
    Directive &operator=(Directive const &other);
    Directive &push_parseable(std::string const &name, IParseable const& parseable);
    Directive &push_parseable(IParseable const& parseable);
    bool parseme(std::string const &str, size_t &idx);
    bool cont_parse(std::string &str, size_t &idx);
    IParseable &operator[](std::string const &);
    IParseable &operator[](size_t idx);
    size_t size();
    IParseable *clone() const;
    std::string &get_string();
    std::set<std::string> &str_set();
    std::vector<std::string> &str_arr();
    virtual int get_int();

};



#endif