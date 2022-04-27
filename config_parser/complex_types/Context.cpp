#include "./Context.hpp"
#include "tools.hpp"
#include "tokens/Single.hpp"

using namespace rgx;

Context::Context(Pattern const &opening, Pattern const &closing, Pattern const &key):
            opening_ptrn(opening),closing_ptrn(closing),key_ptrn(key)
{
    ws_ptrn = Pattern().append(Single(WHITESPACE));
}

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
        this->ws_ptrn = other.ws_ptrn;

        unordered_map<string, IParseable*>::const_iterator it = parseables.begin();
        for ( ; it != parseables.end(); it++)
            delete it->second;
        parseables.clear();

        it = other.parseables.begin();
        for ( ; it != other.parseables.end(); it++)
            insert_parseables(it->first, *it->second);
    }
    return *this;
}

bool Context::parse(string &str, size_t &idx)
{
    unordered_map<string, IParseable *>::iterator it;
    if (opening_ptrn.find(str, idx))
    {
        while (idx < str.size())
        {
            ws_ptrn.find(str, idx); // skip white_space
            key_ptrn.find(str, idx);
            string key = trim(key_ptrn.get_content());
            it = parseables.find(key);
            if (it != parseables.end())
            {
                if (it->second->parse(str, idx) == false)
                    return false;
            }
            else if (closing_ptrn.match(key) == false)
                return false;
            else
                break;
        }
    }
    else 
        return false;
    return true;
}

IParseable &Context::operator[](string const & key) {
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