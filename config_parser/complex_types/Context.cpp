#include "./Context.hpp"
#include "tools.hpp"

using namespace rgx;

Context::Context(Pattern const &opening, Pattern const &closing, Pattern const &key):
            opening_ptrn(opening),closing_ptrn(closing),key_ptrn(key) {}

Context::Context(Context const &other) 
{
    *this = other;
}

Context::~Context() {}

Context &Context::operator=(Context const &other) 
{
    if (this != &other)
    {
        this->opening_ptrn  = other.opening_ptrn;
        this->closing_ptrn = other.closing_ptrn;
        this->key_ptrn = other.key_ptrn;

        unordered_map<string, IParseable*>::const_iterator it = parseables.begin();
        while (it != parseables.end())
            delete it->second;
        parseables.clear();

        it = other.parseables.begin();
        while (it != other.parseables.end())
            insert_parseables(it->first, *it->second);
    }
    return *this;
}

bool Context::parse(string &str, size_t &idx)
{
    unordered_map<string, IParseable *>::iterator it;
    if (opening_ptrn.find(str, idx))
    {
        key_ptrn.find(str, idx);
        string key = trim(key_ptrn.get_content());
        it = parseables.find(key);
        if (it != parseables.end())
        {
            if ( it->second->parse(str, idx) == false)
                return false;
        }
        else if (closing_ptrn.match(key) == false)
            return false;
    }
    else 
        return false;
    return true;
}

IParseable &Context::operator[](string key) {
    unordered_map<string, IParseable *>::iterator it;
    it = parseables.find(key);
    if (it != parseables.end())
        return *it->second;
    throw runtime_error("IParseable key not " + key + " found");
}


Context &Context::insert_parseables(string const &key, IParseable const& parseable)
{
    parseables.insert(pair<string, IParseable *>(key, parseable.clone()));
    return *this;
}

IParseable *Context::clone() const {
    return new Context(*this);
}