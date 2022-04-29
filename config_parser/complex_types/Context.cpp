#include "./Context.hpp"
#include "tools.hpp"
#include "tokens/Single.hpp"

using namespace rgx;

Context::Context(Pattern const &opening, Pattern const &closing, Pattern const &key):
            opening_ptrn(opening),closing_ptrn(closing),key_ptrn(key)
{
    ws_ptrn = Pattern().append(Single(WHITESPACE));
}

Context::Context(Pattern const &opening, Pattern const &closing, Pattern const &key, Pattern const &ws_ptrn):
            opening_ptrn(opening),closing_ptrn(closing),key_ptrn(key), ws_ptrn(ws_ptrn) {}

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

        map<string, IParseable*>::const_iterator it = parseables.begin();
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
    if (opening_ptrn.find(str, idx))
        return core_parse(str, idx);
    return false;
}

bool Context::cont_parse(std::string &str, size_t &idx) 
{
    reached_end = false;
    int key_parse_result;
    if (last_key != "")
    {
        map<string, IParseable *>::iterator it = parseables.find(last_key);
        if (it != parseables.end())
        {
            if (it->second->parse(str, idx) == false)
            {
                reached_end = it->second->is_reached_end();
                return false;
            }
        }
        else if(parseables.find("") != parseables.end())
        {
            IParseable *clone = parseables[""]->clone();
            if (clone->parse(str, idx) == false)
            {   
                delete clone;
                reached_end = clone->is_reached_end();
                return false;
            }
            parseables.insert(std::pair<string, IParseable*>(last_key, clone));
        }
        key_parse_result = parse_by_key(str, idx);
    }
    if (opening_ptrn.find(str, idx))
        key_parse_result = parse_by_key(str, idx);
    if (key_parse_result == BAD_KEY)
        return closing_ptrn.find(str, idx);
    return false;
}

bool Context::core_parse(std::string &str, size_t &idx) 
{
    map<string, IParseable *>::iterator it;
    string key;
    while (idx < str.size())
    {
        ws_ptrn.find(str, idx); // skip white_space
        if (key_ptrn.find(str, idx))
        {
            key = trim(key_ptrn.get_content());
            it = parseables.find(key);
            if (it != parseables.end())
            {
                if (it->second->parse(str, idx) == false)
                {
                    last_key = key;
                    reached_end = it->second->is_reached_end();
                    return false;
                }
            }
            else if(parseables.find("") != parseables.end())
            {
                IParseable *clone = parseables[""]->clone();
                if (clone->parse(str, idx) == false)
                {   
                    delete clone;
                    last_key = key;
                    reached_end = clone->is_reached_end();
                    return false;
                }
                parseables.insert(std::pair<string, IParseable*>(key, clone));
            }
            else if (closing_ptrn.match(key) == false)
                return false;
            else
                break;
        }
    }
    return true;
}

int Context::parse_by_key(std::string &str, size_t &idx) 
{
    map<string, IParseable *>::iterator it;
    string key;
    while (idx < str.size())
    {
        ws_ptrn.find(str, idx); // skip white_space
        if (key_ptrn.find(str, idx))
        {
            key = trim(key_ptrn.get_content());
            it = parseables.find(key);
            if (it != parseables.end())
            {
                if (it->second->parse(str, idx) == false)
                {
                    last_key = key;
                    reached_end = it->second->is_reached_end();
                    return BAD_VALUE;
                }
            }
            else if(parseables.find("") != parseables.end())
            {
                IParseable *clone = parseables[""]->clone();
                if (clone->parse(str, idx) == false)
                {   
                    delete clone;
                    last_key = key;
                    reached_end = clone->is_reached_end();
                    return BAD_VALUE;
                }
                parseables.insert(std::pair<string, IParseable*>(key, clone));
            }
            else
            {
                idx -= key_ptrn.get_content().size();
                break;
            }
        }
        else
            break;
    }
    return BAD_KEY;
}

IParseable &Context::operator[](string const & key) {
    map<string, IParseable *>::iterator it;
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