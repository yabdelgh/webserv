#include "./Single.hpp"

namespace rgx {

    Single::Single(string const &chars, int min, int max):AToken(min, max) {
        this->chars = chars;
    }

    Single::Single(string const &chars, MatchDir match_dir, int min, int max)
                   :AToken(match_dir, min, max)
    {
        this->chars = chars;
    }

    Single::Single(Single const &other) {
        *this = other;
    }

    Single &Single::operator=(Single const &other) {
        if (this != &other) {
            this->chars = other.chars;
            AToken::operator=(other);
        }
        return *this;
    }

    Single::~Single() {}

    bool Single::find(string const &str, size_t &idx, stringstream &ss) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || chars.find(str[idx]) == -1)
                break;
            ss << str[idx];
            idx++;
        }
        return is_matched(i);
    }
    
    bool Single::match(string const &str, size_t &idx) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || chars.find(str[idx]) == -1)
                break;
            idx++;
        }
        // cout << endl << "single match " << i << "  " << min << ":" <<  max << endl;
        return is_matched(i);
    }
    
    AToken *Single::clone() const {
        return (new Single(*this));
    }
}