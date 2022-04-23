#include "./Directive.hpp"

Directive::Directive(rgx::Pattern const &pattern):AComplexType(pattern) {}

Directive::~Directive() {}

bool Directive::parse(std::string &str, size_t &idx)
{
    for (size_t i = 0; i < parseables.size() ; i++)
    {
        if ((*this)[i].parse(str, idx) == false)
            throw std::runtime_error("");
    }
}

IParseable &Directive::operator[](std::string) {
    
}

IParseable &Directive::operator[](size_t idx) {
    
}

