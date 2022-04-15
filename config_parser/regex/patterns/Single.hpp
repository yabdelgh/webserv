#ifndef SINGLE_HPP
#define SINGLE_HPP

#include "./IPattern.hpp"

namespace rgx {
    class Single: public IPattern
    {
    private:
        char c;

    public:
        Single(char c, int min = 1, int max = -1);
        bool parse(std::string const &str, size_t &idx);
        ~Single();
    };
}

# endif