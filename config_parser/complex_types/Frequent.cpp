#include "./Frequent.hpp"

Frequent::Frequent(rgx::Pattern const &pattern):AComplexType(pattern) {}

Frequent::~Frequent() {}

bool Frequent::parse(std::string &str, size_t &idx)
{
    if (parseable->parse(str, idx) == false)
        return false;
    parseables.push_back(parseable);// should clone the parseable
    return true;
}

IParseable &Frequent::operator[](size_t idx) {
    if (idx < parseables.size())
        return *parseables[idx];
    throw std::out_of_range("IParseable out of range index");
}
