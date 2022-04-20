#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "./AToken.hpp"

namespace rgx {
    class Sequence: public AToken
    {
    private:
        std::string seq;
    public:
        Sequence(std::string const &seq, int min = -1, int max = -1);
        Sequence(string const &token);
        ~Sequence();
        bool find(string const &str, size_t &idx, stringstream &ss);
        bool match(string const &str, size_t &idx);
        AToken *clone() const;
    };
}

# endif