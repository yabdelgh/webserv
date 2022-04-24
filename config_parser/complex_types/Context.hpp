#ifndef Context_HPP
#define Context_HPP

#include "./AComplexType.hpp"
#include <unordered_map>

class Context: public AComplexType
{

private:
    std::unordered_map<std::string, IParseable*> parseables;

public:
    Context(rgx::Pattern const &pattern);
    ~Context();
    Context &insert_parseables(std::string const &name, IParseable const& parseable);
    virtual bool parse(std::string &str, size_t &idx);
    virtual IParseable &operator[](std::string);
};



#endif