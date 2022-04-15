#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "./IPattern.hpp"

namespace rgx {
    class Sequence: public IPattern
    {
    private:
        std::string seq;
    public:
        Sequence(std::string const &seq, int min = -1, int max = -1);
        ~Sequence();
        bool parse(std::string const &str, size_t &idx);

    };
}

# endif