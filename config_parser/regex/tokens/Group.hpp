#ifndef GROUP_HPP
# define GROUP_HPP

#include "./ANestedToken.hpp"

namespace rgx {
    class Group : public ANestedToken
    {
    public:
        Group();
        ~Group();
        bool parse(string const &str, size_t &idx, stringstream &ss);
        AToken *clone() const;
    };
}
#endif