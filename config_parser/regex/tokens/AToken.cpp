#include "./AToken.hpp"

namespace rgx {
    AToken::AToken(int min, int max) {
        this->min = min;
        this->max = max;
    }

    AToken::AToken(AToken const &other) {
        *this = other;
    }

    AToken &AToken::operator=(AToken const &other) {
        if (this != &other) {
            this->content = other.content;
            this->min = other.min;
            this->max = other.max;
        }
        return *this;
    }

    AToken::~AToken() {}

    bool AToken::find(string const&str, size_t &idx) {
        stringstream ss;
        if (find(str, idx, ss) == false)
            return false;
        content = ss.str();
        return true;
    }

    bool AToken::get_more(size_t idx) {
        if (min == -1 && max == -1)
            return true;
        if (max > -1)
            return idx < max;
        return true;
    }

    bool AToken::is_matched(size_t times_count) {
        if (min == -1 && max == -1)
            return true;
        if (min == -1)
            return times_count <= max;
        if (max == -1)
            return times_count >= min;
        return between(times_count, min, max);
    }
}