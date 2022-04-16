#include "./APattern.hpp"

namespace rgx {
    APattern::APattern(int min, int max) {
        this->min = min;
        this->max = max;
    }

    APattern::~APattern() {}

    bool APattern::parse(string const&str, size_t &idx) {
        stringstream ss;
        parse(str, idx, ss);
        content = ss.str();
    }
}