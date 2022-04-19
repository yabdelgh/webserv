#include "./Group.hpp"

namespace rgx {
    Group::Group(int min, int max):ANestedToken(min, max)
    {
    }

    Group::~Group()
    {
    }

    bool Group::find(string const &str, size_t &idx, stringstream &ss) {
        stringstream ss;
        for (size_t i = 0; get_more(i) ; i++)
        {
            for (size_t i = 0; i < tokens.size(); i++)
            {
                if (tokens[i]->find(str, idx, ss) == false)
                    break;
            }
            if (i != tokens.size())
                break;
        }
        return is_matched(idx);
    }

    bool Group::match(string const &str, size_t &idx) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            for (size_t i = 0; i < tokens.size(); i++)
            {
                if (tokens[i]->match(str, idx) == false)
                    break;
            }
            if (i != tokens.size())
                break;
        }
        return is_matched(idx);
    }
    
    AToken *Group::clone() const {
        return (new Group());
    }
}