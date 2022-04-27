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
        size_t i,j;
        tmp_idx = idx;
        for (i = 0; get_more(i) ; i++)
        {
            stringstream tmp_ss;
            for (j = 0 ; j < tokens.size(); j++)
            {
                if (tokens[j]->find(str, tmp_idx, tmp_ss) == false)
                    break;
            }
            if (j != tokens.size())
                break;
            ss << tmp_ss.str();
        }
        reached_end = tokens[j]->is_reached_end();
        if (is_matched(i) == false)
            return false;
        idx = tmp_idx;
        return true;
    }

    bool Group::match(string const &str, size_t &idx) {
        size_t i,j;
        tmp_idx = idx;
        for (i = 0; get_more(i) ; i++)
        {
            for (j = 0 ; j < tokens.size(); j++)
            {
                if (tokens[j]->find(str, tmp_idx) == false)
                    break;
            }
            if (j != tokens.size())
                break;
            idx = tmp_idx;
        }
        reached_end = tokens[j]->is_reached_end();
        if (is_matched(i) == false)
            return false;
        idx = tmp_idx;
        return true;
    }
    
    AToken *Group::clone() const {
        return (new Group(*this));
    }
}