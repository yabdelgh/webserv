#ifndef OUT_SEQUENCE_HPP
#define OUT_SEQUENCE_HPP

#include "./AToken.hpp"

namespace rgx {
    class OutSequence: public AToken
    {
    private:
        std::string seq;
    public:
        OutSequence(std::string const &seq, int min = -1, int max = -1);
        OutSequence(string const &token);
        ~OutSequence();
        bool parse(string const &str, size_t &idx, stringstream &ss);

    };
}

# endif