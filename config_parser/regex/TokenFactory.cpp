#include "./TokenFactory.hpp"

namespace rgx {
    TokenFactory* TokenFactory::instance;

    AToken *TokenFactory::new_token(string pattern) {
        
    }

    TokenFactory &TokenFactory::get() {
        if (instance == nullptr) {
            instance = new TokenFactory();
            if (instance == nullptr)
                throw runtime_error("TokenFactory allocation failed!");
        }
        return *instance;
    }

    TokenFactory::TokenFactory() {

        Pattern group_pattern();

        Pattern or_pattern();
        or_pattern().append(OutSequence("|",1))
                    .append(Group().append(OutSequence("|", 1, 1))
                                   .append(Sequence("|", 1 ,1)));
        Pattern between_pattern();
        between_pattern().append(Between(' ', '~', 1))
                         .append(Single("-", 1, 1))
                         .append(Between(' ', '~', 1));

        Pattern sequence_pattern();   
        
        patterns.push_back(or_pattern);
    }

    TokenFactory::~TokenFactory() {}
}