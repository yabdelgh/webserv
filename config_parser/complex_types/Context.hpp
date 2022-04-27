#ifndef Context_HPP
#define Context_HPP

#include "./AComplexType.hpp"
#include <unordered_map>

class Context: public AComplexType
{

private:
    rgx::Pattern opening_ptrn;
    rgx::Pattern closing_ptrn;
    rgx::Pattern key_ptrn;
    rgx::Pattern ws_ptrn;
    std::unordered_map<std::string, IParseable*> parseables;

public:
    Context(rgx::Pattern const &opening, rgx::Pattern const &closing, rgx::Pattern const &key);
    Context(Context const &other);
    ~Context();
    Context &operator=(Context const &other);
    Context &insert_parseables(std::string const &name, IParseable const& parseable);
    bool parse(std::string &str, size_t &idx);
    // bool cont_parse(std::string &str, size_t &idx);
    IParseable &operator[](std::string const &);
    IParseable *clone() const;
};



#endif