#ifndef SINGLE_HPP
#define SINGLE_HPP

#include "./AToken.hpp"

namespace rgx {
    class Single: public AToken
    {
    private:
        string chars;

    public:
        Single(string const &chars, int min = -1, int max = -1);
        Single(Single const &);
        Single &operator=(Single const &);
        ~Single();
        bool find(string const &str, size_t &idx, stringstream &ss);
        bool match(string const &str, size_t &idx);
        AToken *clone() const;
    };
}

# endif
