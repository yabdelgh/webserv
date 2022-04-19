# include "./ANestedToken.hpp"

namespace rgx {
    ANestedToken::ANestedToken(int min, int max):AToken(min, max) {}
    ANestedToken::~ANestedToken() {}
    ANestedToken &ANestedToken::append(AToken const &token) {
        tokens.push_back(token.clone());
        return *this;
    }
}