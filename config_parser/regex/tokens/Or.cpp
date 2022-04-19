#include "./Or.hpp"

namespace rgx {
    Or::Or(int min, int max):ANestedToken(min, max) {}

    Or::~Or()
    {
    }

    bool Or::find(string const &str, size_t &idx, stringstream &ss) {
        stringstream ss;
        for (size_t i = 0; get_more(i) ; i++)
        {
            for (size_t i = 0; i < tokens.size(); i++)
            {
                if (tokens[i]->find(str, idx, ss) == true)
                    break;
            }
            if (i == tokens.size())
                break;
        }
        return is_matched(idx);
    }

    bool Or::match(string const &str, size_t &idx) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            for (size_t i = 0; i < tokens.size(); i++)
            {
                if (tokens[i]->match(str, idx) == true)
                    break;
            }
            if (i == tokens.size())
                break;
        }
        return is_matched(idx);
    }

    AToken *Or::clone() const {
        return (new Or());
    }
}