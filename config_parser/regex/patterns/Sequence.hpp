#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "./APattern.hpp"

namespace rgx {
    class Sequence: public APattern
    {
    private:
        std::string seq;
    public:
        Sequence(std::string const &seq, int min = -1, int max = -1);
        ~Sequence();
        bool parse(string const&str, size_t &idx) = 0;
        bool parse(string const &str, size_t &idx, stringstream &ss);

    };
}

# endif