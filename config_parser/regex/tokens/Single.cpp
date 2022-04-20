#include "./Single.hpp"

namespace rgx {

    Single::Single(string const &chars, int min, int max) {
        this->chars = chars;
        this->min = min;
        this->max = (max == -1 ? min : max);
    }

    bool Single::find(string const &str, size_t &idx, stringstream &ss) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || chars.find(str[idx]) == -1)
                break;
            ss << str[idx];
            idx++;
        }
        return is_matched(idx);
    }
    
    bool Single::match(string const &str, size_t &idx) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || chars.find(str[idx]) == -1)
                break;
            idx++;
        }
        return is_matched(idx);
    }
    
    AToken *Single::clone() const {
        return (new Single());
    }
}