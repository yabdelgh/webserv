#include "./Or.hpp"

namespace rgx {
    Or::Or(int min, int max):ANestedToken(min, max) {}

    Or::Or(Or const &other) {
        *this = other;
    }

    Or &Or::operator=(Or const &other) {
        if (this != &other) {
            ANestedToken::operator=(other);
        }
        return *this;
    }

    Or::~Or() {}

    bool Or::find(string const &str, size_t &idx, stringstream &ss) {
        size_t i, j;
        tmp_idx = idx;
        for (i = 0; get_more(i) ; i++)
        {
            stringstream tmp_ss;
            for (j = 0 ; j < tokens.size(); j++)
            {
                if (tokens[j]->find(str, tmp_idx, tmp_ss) == true)
                    break;
                reached_end |= tokens[j]->is_reached_end();
            }
            if (j == tokens.size())
                break;
            ss << tmp_ss;
        }
        if (is_matched(i) == false)
            return false;
        idx = tmp_idx;
        return true;
    }

    bool Or::match(string const &str, size_t &idx) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            for (size_t i = 0; i < tokens.size(); i++)
            {
                if (tokens[i]->match(str, tmp_idx) == true)
                    break;
            }
            if (i == tokens.size())
                break;
        }
        if (is_matched(i) == false)
            return false;
        idx = tmp_idx;
        return true;
    }

    AToken *Or::clone() const {
        return (new Or(*this));
    }
}