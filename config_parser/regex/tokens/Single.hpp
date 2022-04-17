#ifndef SINGLE_HPP
#define SINGLE_HPP

#include "./AToken.hpp"

namespace rgx {
    class Single: public AToken
    {
    private:
        char c;

    public:
        Single(char c, int min = 1, int max = -1);
        ~Single();
        bool parse(string const &str, size_t &idx, stringstream &ss);
    };
}

# endif