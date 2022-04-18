#include "./OutSequence.hpp"

namespace rgx {

    OutSequence::OutSequence(std::string const &seq, int min, int max):seq(seq) {
        this->min = min;
        this->max = (max == -1 ? min : max);
    }

    bool OutSequence::parse(std::string const &str, size_t &idx, stringstream &ss) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (seq.compare(0, seq.size(), str) == 0)
                break;
            ss << seq;
            idx += seq.size();
        }
        return is_matched(idx);
    }
}