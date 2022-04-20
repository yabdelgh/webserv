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
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            for (size_t i = 0; i < tokens.size(); i++)
            {
                if (tokens[i]->find(str, idx, ss) == true)
                    break;
            }
            if (i == tokens.size())
                break;
        }
        return is_matched(i);
    }

    bool Or::match(string const &str, size_t &idx) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            for (size_t i = 0; i < tokens.size(); i++)
            {
                if (tokens[i]->match(str, idx) == true)
                    break;
            }
            if (i == tokens.size())
                break;
        }
        return is_matched(i);
    }

    AToken *Or::clone() const {
        return (new Or(*this));
    }
}