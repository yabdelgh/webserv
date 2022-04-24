#include "./Directive.hpp"

Directive::Directive(rgx::Pattern const &pattern):AComplexType(pattern) {}

Directive::~Directive() {}

bool Directive::parse(std::string &str, size_t &idx)
{
    for (size_t i = 0; i < parseables.size() ; i++)
    {
        if ((*this)[i].parse(str, idx) == false)
            return false;
    }
    return true;
}

IParseable &Directive::operator[](std::string name) {
    for (size_t i = 0; i < parseables.size() ; i++)
    {
        if (parseables[i].first == name)
            return *parseables[i].second;
    }
    throw std::runtime_error("IParseable key not " + name + " found");
}

IParseable &Directive::operator[](size_t idx) {
    if (idx < parseables.size())
        return *parseables[idx].second;
    throw std::out_of_range("IParseable out of range index");
}

Directive &Directive::push_parseable(std::string const &name, IParseable const& parseable)
{
    parseables.push_back(std::pair<std::string, IParseable *>(name, &parseable));
    return *this;
}

Directive &Directive::push_parseable(IParseable const& parseable)
{
    parseables.push_back(std::pair<std::string, IParseable *>("", &parseable));
    return *this;
}