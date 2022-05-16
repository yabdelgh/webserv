#include "./Frequent.hpp"

Frequent::Frequent(IParseable const &parseable):parseable(parseable.clone()) {}

Frequent::Frequent(Frequent const &other):parseable(nullptr)
{
    *this = other;
}

Frequent &Frequent::operator=(Frequent const &other) {
    if (this != &other) 
    {
        delete this->parseable;
        this->parseable = other.parseable->clone();
        for (size_t i = 0 ; i < parseables.size() ; i++)
        {
            delete parseables[i];
        }
        parseables.clear();
        for (size_t i = 0 ; i < other.parseables.size() ; i++)
        {
            parseables.push_back(other.parseables[i]->clone());
        }
    }
    return *this;
}

Frequent::~Frequent() {}

bool Frequent::parseme(std::string const &str, size_t &idx)
{
    IParseable *clone = parseable->clone();
    if (clone->parse(str, idx) == false)
    {
        reached_end = parseable->is_reached_end();
        delete clone;  
        return false;
    }
    parseables.push_back(clone);
    return true;
}

IParseable &Frequent::operator[](size_t idx) {
    if (idx < parseables.size())
        return *parseables[idx];
    throw std::out_of_range("IParseable out of range index");
}

size_t Frequent::size() const{
    return parseables.size();
}

IParseable *Frequent::clone() const 
{
    return new Frequent(*this);
}

std::vector<IParseable*> &Frequent::get_parseables()
{
    return parseables;
}