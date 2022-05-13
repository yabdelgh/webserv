#ifndef Context_HPP
#define Context_HPP

#include "./AComplexType.hpp"
#include <map>

struct CaseInsensitiveCom
{
    bool operator()(const std::string& a, const std::string& b) const
    {
        return ::strcasecmp(a.c_str(), b.c_str()) < 0;
    }
};

class Context: public AComplexType
{

enum {
    BAD_KEY,
    BAD_VALUE,
    GOOD_VALUE
};

enum PauseState{
    OPENING,
    PAIRS_PARSE,
    ENDING,
};

private:
    rgx::Pattern opening_ptrn;
    rgx::Pattern closing_ptrn;
    rgx::Pattern key_ptrn;
    rgx::Pattern ws_ptrn;
    PauseState pause_state;
    std::string last_key;
    std::map<std::string, IParseable*, CaseInsensitiveCom> parseables;
    std::set<std::string, CaseInsensitiveCom> dflt_parseables;
    int parse_by_key(std::string &str, size_t &idx);
    inline int parse_one(std::string &str, size_t &idx);

public:
    Context(rgx::Pattern const &opening, rgx::Pattern const &closing, rgx::Pattern const &key);
    Context(rgx::Pattern const &opening, rgx::Pattern const &closing, rgx::Pattern const &key, rgx::Pattern const &ws_ptr);
    Context(Context const &other);
    ~Context();
    Context &operator=(Context const &other);
    Context &insert_parseables(std::string const &name, IParseable const& parseable, bool dflt = false);
    bool parseme(std::string const &str, size_t &idx);
    bool cont_parse(std::string &str, size_t &idx);
    bool core_parse(std::string &str, size_t &idx);
    IParseable &operator[](std::string const &);
    IParseable *clone() const;

};



#endif