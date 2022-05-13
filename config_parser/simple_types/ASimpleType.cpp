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

bool ASimpleType::parseme(std::string const &str, size_t &idx)
{
    if (pattern.find(str, idx))
    {
        std::cout << "find :|" << pattern.get_content() << "|" << std::endl;
        set_value(pattern.get_content());
        return true;
    }
    reached_end = pattern.is_reached_end();
    return false;
}