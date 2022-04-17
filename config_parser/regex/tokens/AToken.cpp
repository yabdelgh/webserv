#include "./AToken.hpp"

namespace rgx {
    AToken::AToken(int min, int max) {
        this->min = min;
        this->max = max;
    }

    AToken::~AToken() {}

    bool AToken::parse(string const&str, size_t &idx) {
        stringstream ss;
        parse(str, idx, ss);
        content = ss.str();
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