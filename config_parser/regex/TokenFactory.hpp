#ifndef TOKEN_FACTORY_HPP
# define TOKEN_FACTORY_HPP

#include "./tokens/Sequence.hpp"
#include "./tokens/OutSequence.hpp"
#include "./tokens/Between.hpp"
#include "./tokens/Or.hpp"
#include "./tokens/Group.hpp"
#include "./Pattern.hpp"
#include <vector>



namespace rgx
{
    enum token_type {
        SEQUENCE,
        OutSEQUENCE,
        BETWEEN, 
        OR,
    };

    class TokenFactory
    {
    private:
        static TokenFactory *instance;
        vector<struct Pattern> patterns;

    private:
        TokenFactory();

    public:
        AToken *new_token(string pattern);
        TokenFactory &get();
        ~TokenFactory();
    };
}

#endif