#include "./Context.hpp"
#include "tools.hpp"
#include "tokens/Single.hpp"
#include <strings.h>

using namespace rgx;

Context::Context(Pattern const &opening, Pattern const &closing, Pattern const &key):
            opening_ptrn(opening),closing_ptrn(closing),key_ptrn(key)
{
    reached_end = false;
    pause_state = OPENING;
    ws_ptrn = Pattern().append(Single(WHITESPACE));
}

Context::Context(Pattern const &opening, Pattern const &closing, Pattern const &key, Pattern const &ws_ptrn):
            opening_ptrn(opening),closing_ptrn(closing),key_ptrn(key), ws_ptrn(ws_ptrn)
{
    reached_end = false;
    pause_state = OPENING;
}

Context::Context(Context const &other) 
{
    *this = other;
}

Context::~Context()
{
    map<string, IParseable*>::const_iterator it = parseables.begin();
    for ( ; it != parseables.end(); it++)
        delete it->second;
    parseables.clear();
}

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
        dflt_parseables = other.dflt_parseables;
    }
    return *this;
}

bool Context::parseme(string const &str, size_t &idx)
{
    map<string, IParseable *>::iterator it;
    map<string, IParseable *>::iterator end;
    string key;

    if (opening_ptrn.find(str, idx))
    {
        while (idx < str.size())
        {
            ws_ptrn.find(str, idx); // skip white_space
            if (key_ptrn.find(str, idx))
            {
                key = trim(key_ptrn.get_content());
                end = parseables.end();
                if ((it = parseables.find(key)) != end || (it = parseables.find("")) != end)
                {
                    if (it->first == "")
                    {
                        parseables[key] = it->second->clone();
                        it = parseables.find(key);
                    }
                    if (it->second->parse(str, idx) == false)
                        return false;
                    continue;
                }
            }
            idx -= key_ptrn.get_content().size();
            break;
        }
        return closing_ptrn.find(str, idx);
    }
    return false;
}

bool Context::cont_parse(std::string &str, size_t &idx) 
{
   
    bool matched;

    if (reached_end == false)
        pause_state = OPENING;
    switch (pause_state)
    {
    case OPENING:
        matched = opening_ptrn.find(str, idx);
        reached_end = opening_ptrn.is_reached_end();
        if (!matched)
            break;
        pause_state = PAIRS_PARSE;

    case PAIRS_PARSE:
        int parse_stat;
        do
        {
            parse_stat = parse_one(str, idx);
        }while(parse_stat == GOOD_VALUE);
        if (parse_by_key(str, idx) == BAD_VALUE)
            break;
        pause_state = ENDING;

    case ENDING:
        matched = closing_ptrn.find(str, idx);
        reached_end = closing_ptrn.is_reached_end();
        if (!matched)
            break;
        pause_state = OPENING;
        return true;
    }
    return false;
}

int Context::parse_by_key(std::string &str, size_t &idx) 
{
    map<string, IParseable *>::iterator it;
    map<string, IParseable *>::iterator end;
    string key;
    if (last_key != "")
    {
        key = last_key;
        // goto parsing;
    }
    while (idx < str.size())
    {
        ws_ptrn.find(str, idx); // skip white_space
        if (key_ptrn.find(str, idx))
        {
            key = trim(key_ptrn.get_content());
            end = parseables.end();
            if ((it = parseables.find(key)) != end || (it = parseables.find("")) != end)
            {
                if (it->first == "")
                {
                    parseables[key] = it->second->clone();
                    it = parseables.find(key);
                }
                if (it->second->cont_parse(str, idx) == false)
                {
                    reached_end = it->second->is_reached_end();
                    last_key = key;
                    return BAD_VALUE;
                }
                continue;
            }
            idx -= key_ptrn.get_content().size();
            break;
        }
        else
            break;
    }
    return BAD_KEY;
}

int Context::parse_one(std::string &str, size_t &idx)
{
    map<string, IParseable *>::iterator it;
    map<string, IParseable *>::iterator end;
    string key;
    
    if (key_ptrn.find(str, idx))
    {
        key = trim(key_ptrn.get_content());
        end = parseables.end();
        if ((it = parseables.find(key)) != end || (it = parseables.find("")) != end)
        {
            if (it->first == "")
            {
                parseables[key] = it->second->clone();
                it = parseables.find(key);
            }
            if (it->second->cont_parse(str, idx) == false)
            {
                reached_end = it->second->is_reached_end();
                last_key = key;
                return BAD_VALUE;
            }
            return GOOD_VALUE;
        }
    }
    idx -= key_ptrn.get_content().size();
    return BAD_KEY;
}

IParseable &Context::operator[](string const & key) {
    map<string, IParseable *>::iterator it;
    it = parseables.find(key);
    if (it != parseables.end() && (it->second->is_parsed() ||
                            dflt_parseables.find(key) != dflt_parseables.end()))
    {
        return *it->second;
    }
    throw runtime_error("IParseable key:" + key + " not found.");
}


Context &Context::insert_parseables(string const &key,
                                    IParseable const& parseable,
                                    bool dflt)
{
    parseables.insert(make_pair(key, parseable.clone()));
    if (dflt)
        dflt_parseables.insert(key);
    return *this;
}

IParseable *Context::clone() const {
    return new Context(*this);
}
