#include "./Context.hpp"

Context::Context(rgx::Pattern const &pattern):AComplexType(pattern) {}

Context::~Context() {}

bool Context::parse(std::string &str, size_t &idx)
{
    // parse the context opening
    // parse key form 
    //      while parse key equal to one of the keys from the list
    //          parse item by key
    // parse the context closing 
    return true;
}

IParseable &Context::operator[](std::string key) {
    std::unordered_map<std::string, IParseable *>::iterator it;
    it = parseables.find(key);
    if (it != parseables.end())
        return *it->second;
    throw std::runtime_error("IParseable key not " + key + " found");
}


Context &Context::insert_parseables(std::string const &key, IParseable const& parseable)
{
    parseables.insert(std::pair<std::string, IParseable *>(key, &parseable));
    return *this;
}
