#include "./Single.hpp"

namespace rgx {

    Single::Single(char c, int min, int max) {
        this->c = c;
        this->min = min;
        this->max = (max == -1 ? min : max);
    }

    bool Single::find(string const &str, size_t &idx, stringstream &ss) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || c != str[idx])
                break;
            ss << str[idx];
            idx++;
        }
        return is_matched(idx);
    }
    
    bool Single::match(string const &str, size_t &idx) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || c != str[idx])
                break;
            idx++;
        }
        return is_matched(idx);
    }
}