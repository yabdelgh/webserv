#include "./Group.hpp"

namespace rgx {
    Group::Group(/* args */)
    {
    }

    Group::~Group()
    {
    }

    bool Group::parse(string const &str, size_t &idx, stringstream &ss) {
        
    }

    AToken *Group::clone() const {
        return (new Group());
    }
}