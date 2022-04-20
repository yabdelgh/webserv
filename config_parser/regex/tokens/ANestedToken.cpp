# include "./ANestedToken.hpp"

namespace rgx {
    ANestedToken::ANestedToken(int min, int max):AToken(min, max) {}
    
    ANestedToken::ANestedToken(ANestedToken const &other) {
        *this = other;
    }

    ANestedToken &ANestedToken::operator=(ANestedToken const &other) {
        if (this != &other) {
            tokens.clear();
            vector<AToken *>::const_iterator it = other.tokens.begin();
            vector<AToken *>::const_iterator end = other.tokens.end();
            for (; it != end ; it++)
                tokens.push_back((*it)->clone());
            ANestedToken::operator=(other);
        }
        return *this;
    }

    ANestedToken::~ANestedToken() {}
    ANestedToken &ANestedToken::append(AToken const &token) {
        tokens.push_back(token.clone());
        return *this;
    }
}