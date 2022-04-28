#include "./Directive.hpp"

using namespace rgx;

Directive::Directive():last_index(0) {}

Directive::Directive(Directive const &other)
{
    *this = other;
}

Directive &Directive::operator=(Directive const &other)
{
    if (this != &other)
    {
        last_index = other.last_index;
        for (size_t i = 0 ; i < parseables.size() ; i++)
        {
            delete parseables[i].second;
        }
        parseables.clear();
        for (size_t i = 0 ; i < other.parseables.size() ; i++)
        {
            push_parseable(other.parseables[i].first, *other.parseables[i].second);
        }
    }
    return *this;
}

Directive::~Directive() {}

bool Directive::parse(string &str, size_t &idx)
{
    for (size_t i = 0; i < parseables.size() ; i++)
    {
        if ((*this)[i].parse(str, idx) == false)
        {
            reached_end = (*this)[i].is_reached_end();
            return false;
        }
    }
    return true;
}

bool Directive::cont_parse(std::string &str, size_t &idx)
{   
    size_t i = last_index;
    for (; i < parseables.size() ; i++)
    {
        if ((*this)[i].parse(str, idx) == false)
        {
            reached_end = (*this)[i].is_reached_end();
            return false;
        }
    }
    if (i == parseables.size())
        last_index = 0;
    return true;
}

IParseable &Directive::operator[](string const &name) {
    for (size_t i = 0; i < parseables.size() ; i++)
    {
        if (parseables[i].first == name)
            return *parseables[i].second;
    }
    throw runtime_error("IParseable key not " + name + " found");
}

IParseable &Directive::operator[](size_t idx) {
    if (idx < parseables.size())
        return *parseables[idx].second;
    throw out_of_range("IParseable out of range index");
}

Directive &Directive::push_parseable(string const &name, IParseable const& parseable)
{
    parseables.push_back(pair<string, IParseable *>(name, parseable.clone()));
    return *this;
}

Directive &Directive::push_parseable(IParseable const& parseable)
{
    parseables.push_back(pair<string, IParseable *>("", parseable.clone()));
    return *this;
}

size_t Directive::size() {
    return parseables.size();
}

IParseable *Directive::clone() const
{
    return new Directive(*this);
}