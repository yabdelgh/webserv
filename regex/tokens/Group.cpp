#include "./Group.hpp"

namespace rgx {
    Group::Group(int min, int max):ANestedToken(min, max)
    {
    }

    Group::Group(Group const &other) {
        *this = other;
    }

    Group &Group::operator=(Group const &other) {
        if (this != &other) {
            ANestedToken::operator=(other);
        }
        return *this;
    }

    Group::~Group() {}

    bool Group::find(string const &str, size_t &idx, stringstream &ss) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            stringstream tmp_ss;
            size_t j = 0;
            for (; j < tokens.size(); j++)
            {
                if (tokens[j]->find(str, idx, tmp_ss) == false)
                    break;
            }
            if (j != tokens.size())
                break;
            ss << tmp_ss.str();
        }
        return is_matched(i);
    }

    bool Group::match(string const &str, size_t &idx) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            for (size_t i = 0; i < tokens.size(); i++)
            {
                if (tokens[i]->match(str, idx) == false)
                    break;
            }
            if (i != tokens.size())
                break;
        }
        return is_matched(i);
    }
    
    AToken *Group::clone() const {
        return (new Group(*this));
    }
}