#ifndef BETWEEN_HPP
#define BETWEEN_HPP

#include "./AToken.hpp"

namespace rgx {
    class Between: public AToken
    {
    private:
        char c_min;
        char c_max;
    public:
        Between(char c_min, char c_max, int min = -1, int max = -1);
        Between(string const &token);
        Between(Between const &);
        Between &operator=(Between const &);
        ~Between();
        bool find(string const &str, size_t &idx, stringstream &ss);
        bool match(string const &str, size_t &idx);
        AToken *clone() const;
    };
}

# endif