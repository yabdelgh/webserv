#ifndef OR_HPP
# define OR_HPP

#include "./ANestedToken.hpp"

namespace rgx {
    class Or : public ANestedToken
    {
    public:
        Or(int min = -1, int max = -1);
        ~Or();
        bool find(string const &str, size_t &idx, stringstream &ss);
        bool match(string const &str, size_t &idx);
        AToken *clone() const;
    };
}
#endif