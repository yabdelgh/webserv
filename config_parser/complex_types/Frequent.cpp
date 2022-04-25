#include "./Frequent.hpp"


Frequent::Frequent(IParseable const &parseable):parseable(parseable.clone()) {}

Frequent::Frequent(Frequent const &other) 
{
    *this = other;
}

Frequent &Frequent::operator=(Frequent const &other) {
    if (this != &other) 
    {
        delete this->parseable;
        this->parseable = other.parseable->clone();
        AComplexType::operator=(other);
    }
    return *this;
}

Frequent::~Frequent() {}

bool Frequent::parse(std::string &str, size_t &idx)
{
    if (parseable->parse(str, idx) == false)
        return false;
    parseables.push_back(parseable->clone());
    return true;
}

IParseable &Frequent::operator[](size_t idx) {
    if (idx < parseables.size())
        return *parseables[idx];
    throw std::out_of_range("IParseable out of range index");
}

size_t Frequent::size() {
    return parseables.size();
}

IParseable *Frequent::clone() const 
{
    return new Frequent(*this);
}