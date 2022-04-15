#include "./Between.hpp"


namespace rgx {
    inline bool between(char c, char c_min, char c_max) {
        return (c >= c_min && c<= c_max);
    }
    Between::Between(char c_min, char c_max, int min, int max) {
        this->c_min = c_min;
        this->c_max = c_max;
        this->min = min;
        this->max = (max == -1 ? min : max);
    }

    bool Between::parse(std::string const &str, size_t &idx) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || !between(str[idx], c_min, c_max)) {
                return false;
            }
            idx++;
        }
        return true;
    }
}