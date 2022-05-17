#include "./ANestedToken.hpp"

namespace rgx
{
    ANestedToken::ANestedToken(int min, int max) : AToken(min, max) {}

    ANestedToken::ANestedToken(ANestedToken const &other)
    {
        *this = other;
    }

    ANestedToken::~ANestedToken()
    {
        clear();
    }

    void ANestedToken::clear()
    {
        vector<AToken *>::const_iterator it = tokens.begin();
        vector<AToken *>::const_iterator end = tokens.end();
        for (; it != end; it++)
            delete (*it);
        tokens.clear();
    }

    ANestedToken &ANestedToken::operator=(ANestedToken const &other)
    {
        if (this != &other)
        {
            clear();
            vector<AToken *>::const_iterator it = other.tokens.begin();
            vector<AToken *>::const_iterator end = other.tokens.end();
            for (; it != end; it++)
                tokens.push_back((*it)->clone());
            AToken::operator=(other);
        }
        return *this;
    }

    ANestedToken &ANestedToken::append(AToken const &token)
    {
        tokens.push_back(token.clone());
        return *this;
    }
}