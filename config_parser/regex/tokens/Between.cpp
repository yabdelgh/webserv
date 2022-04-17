#include "./Between.hpp"


namespace rgx {
    Between::Between(char c_min, char c_max, int min, int max) {
        this->c_min = c_min;
        this->c_max = c_max;
        this->min = min;
        this->max = (max == -1 ? min : max);
    }

    Between::Between(string const &ptrn, size_t &idx) {
        size_t i = idx;
        if ((ptrn.size() - idx) >= 5)
        {
            bool condition = ptrn[idx] == '[' &&
                             ptrn[idx + 2] == '-' &&
                             ptrn[idx + 4] == ']' &&
                             between(ptrn[idx + 1], ' ', '~') &&
                             between(ptrn[idx + 3], ' ', '~');
            c_min = ptrn[idx + 1];
            c_max = ptrn[idx + 3];
            idx += 5
            set_boundry(ptrn, idx);
        }
    }

    bool Between::parse(std::string const &str, size_t &idx, stringstream &ss) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || !between(str[idx], c_min, c_max))
                break;
            ss << str[idx];
            idx++;
        }
        return is_matched(idx);
    }
}