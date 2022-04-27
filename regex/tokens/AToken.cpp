#include "./AToken.hpp"

namespace rgx {
    AToken::AToken(int min, int max) {
        this->match_dir = MATCH_IN;
        set_min(min);
        set_max(max);
    }

    AToken::AToken(MatchDir match_dir, int min, int max) {
        this->match_dir = match_dir;
        set_min(min);
        set_max(max);
    }

    AToken::AToken(AToken const &other) {
        *this = other;
    }

    AToken &AToken::operator=(AToken const &other) {
        if (this != &other) {
            this->content = other.content;
            this->min = other.min;
            this->max = other.max;
            this->match_dir = other.match_dir;
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

    string const &AToken::get_content() const {
        return content;
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

    AToken &AToken::set_min(int min) {
        if (min < 0)
            this->min = -1;
        else
            this->min = min;
        return *this;
    }
    
    AToken &AToken::set_max(int max) {
        if (max < 0)
            this->max = -1;
        else
            this->max = max;
        return *this;
    }
}