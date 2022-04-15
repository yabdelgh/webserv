#ifndef BETWEEN_HPP
#define BETWEEN_HPP

#include "./IPattern.hpp"

namespace rgx {
    class Between: public IPattern
    {
    private:
        char c_min;
        char c_max;
    public:
        Between(char c_min, char c_max, int min = -1, int max = -1);
        ~Between();
        bool parse(std::string const &str, size_t &idx);
    };
}

# endif