#include "./Or.hpp"

namespace rgx {
    Or::Or(/* args */)
    {
    }

    Or::~Or()
    {
    }

    bool Or::parse(string const &str, size_t &idx, stringstream &ss) {
        
    }

    AToken *Or::clone() const {
        return (new Or());
    }
}