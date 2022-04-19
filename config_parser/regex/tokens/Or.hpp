#ifndef OR_HPP
# define OR_HPP

#include "./ANestedToken.hpp"

namespace rgx {
    class Or : public ANestedToken
    {
    public:
        Or();
        ~Or();
        bool parse(string const &str, size_t &idx, stringstream &ss);
        AToken *clone() const;
    };

    Or::Or(/* args */)
    {
    }

    Or::~Or()
    {
    }
}
#endif