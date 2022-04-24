#include "./ASimpleType.hpp"

ASimpleType::ASimpleType(rgx::Pattern const &pattern):pattern(pattern) {}

ASimpleType::~ASimpleType() {}

bool ASimpleType::parse(std::string &str, size_t &idx)
{
    if (pattern.find(str, idx))
    {
        set_value(pattern.get);
        return true;
    }
    return false;
}