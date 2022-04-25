#include "./Between.hpp"


namespace rgx {
    Between::Between(char c_min, char c_max, int min, int max):AToken(min, max) {
        this->c_min = c_min;
        this->c_max = c_max;
    }

    Between::Between(Between const &other) {
        *this = other;
    }

    Between &Between::operator=(Between const &other) {
        if (this != &other) {
            this->c_max = other.c_max;
            this->c_min = other.c_min;
            AToken::operator=(other);
        }
        return *this;
    }

    Between::~Between() {}

    bool Between::find(std::string const &str, size_t &idx, stringstream &ss) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || !between(str[idx], c_min, c_max))
                break;
            ss << str[idx];
            idx++;
        }
        return is_matched(i);
    }

    bool Between::match(string const &str, size_t &idx) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            if (idx >= str.size() || !between(str[idx], c_min, c_max))
                break;
            idx++;
        }
        // cout << endl << "between match " << i << "  " << min << ":" <<  max << endl;
        return is_matched(i);
    }

    AToken *Between::clone() const {
        return (new Between(*this));
    }
}