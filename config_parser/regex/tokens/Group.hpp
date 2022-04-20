#ifndef GROUP_HPP
# define GROUP_HPP

#include "./ANestedToken.hpp"

namespace rgx {
    class Group : public ANestedToken
    {
    public:
        Group(int min = -1, int max = -1);
        Group(Group const &);
        Group &operator=(Group const &);
        ~Group();
        bool find(string const &str, size_t &idx, stringstream &ss);
        bool match(string const &str, size_t &idx);
        AToken *clone() const;
    };
}
#endif