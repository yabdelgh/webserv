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
        size_t tmp_idx = idx;
        for (i = 0; get_more(i) ; i++)
        {
            if (tmp_idx >= str.size() || (chars.find(str[tmp_idx]) == -1) ^ match_dir)
                break;
            ss << str[tmp_idx];
            tmp_idx++;
        }
        if (is_matched(i))
        {
            idx = tmp_idx;
            return true;
        }
        return false;
    }
    
    bool Single::match(string const &str, size_t &idx) {
        size_t i;
        size_t tmp_idx = idx;
        for (i = 0; get_more(i) ; i++)
        {
            if (tmp_idx >= str.size() || (chars.find(str[tmp_idx]) == -1) ^ match_dir)
                break;
            tmp_idx++;
        }
        if (is_matched(i))
        {
            idx = tmp_idx;
            return true;
        }
        return false;
    }
    
    AToken *Single::clone() const {
        return (new Single(*this));
    }
}