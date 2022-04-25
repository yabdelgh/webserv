#include "./ASimpleType.hpp"

ASimpleType::ASimpleType() {}

ASimpleType::ASimpleType(rgx::Pattern const &pattern):pattern(pattern) {}

ASimpleType::ASimpleType(ASimpleType const &other) {
    *this = other;
}

ASimpleType::~ASimpleType() {}

ASimpleType &ASimpleType::operator=(ASimpleType const &other)
{
    if (this != &other)
        this->pattern = other.pattern;
    return *this;
}

bool ASimpleType::parse(std::string &str, size_t &idx)
{
    if (pattern.find(str, idx))
    {
        set_value(pattern.get_content());
        return true;
    }
    return false;
}