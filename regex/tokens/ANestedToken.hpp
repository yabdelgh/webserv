#ifndef ANESTED_TOKEN_HPP
#define ANESTED_TOKEN_HPP

# include "./AToken.hpp"
# include <vector>

namespace rgx {
    class ANestedToken: public AToken
    {
    protected:
        vector<AToken *> tokens;
    public:
        ANestedToken(int min = -1, int max = -1);
        ANestedToken(ANestedToken const &);
        ANestedToken &operator=(ANestedToken const &);
        ~ANestedToken();
        ANestedToken &append(AToken const &token);
    };
}

# endif