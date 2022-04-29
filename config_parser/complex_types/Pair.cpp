#include "./Pair.hpp"

using namespace rgx;

Pair::Pair(Pattern const &opening, Pattern const &closing, Pattern const &key):
            opening_ptrn(opening),closing_ptrn(closing),key_ptrn(key)
{
    ws_ptrn = Pattern().append(Single(WHITESPACE));
}

Pair::Pair(Pair const &other) 
{
    *this = other;
}

Pair::~Pair() {}

Pair &Pair::operator=(Pair const &other) 
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

bool Pair::parse(string &str, size_t &idx)
{
    if (opening_ptrn.find(str, idx))
        return core_parse(str, idx);
    return false;
}

bool Pair::cont_parse(std::string &str, size_t &idx) 
{
    reached_end = false;
    if (last_key != "")
    {
        unordered_map<string, IParseable *>::iterator it = parseables.find(last_key);
        if (it != parseables.end())
        {
            if (it->second->parse(str, idx) == false)
            {
                reached_end = it->second->is_reached_end();
                return false;
            }
        }
        return core_parse(str, idx);
    }
    if (opening_ptrn.find(str, idx))
        return core_parse(str, idx);
    return false;
}

bool Pair::core_parse(std::string &str, size_t &idx) 
{
    unordered_map<string, IParseable *>::iterator it;
    string key;
    while (idx < str.size())
    {
        ws_ptrn.find(str, idx); // skip white_space
        key_ptrn.find(str, idx);
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
        else if (closing_ptrn.match(key) == false)
            return false;
        else
            break;
    }
    return true;
}

IParseable &Pair::operator[](string const & key) {
    unordered_map<string, IParseable *>::iterator it;
    it = parseables.find(key);
    if (it != parseables.end())
        return *it->second;
    throw runtime_error("IParseable key not " + key + " found");
}


Pair &Pair::insert_parseables(string const &key, IParseable const& parseable)
{
    parseables.insert(pair<string, IParseable *>(key, parseable.clone()));
    return *this;
}

IParseable *Pair::clone() const {
    return new Pair(*this);
}