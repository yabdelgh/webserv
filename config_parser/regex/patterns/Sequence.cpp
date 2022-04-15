#include "./Sequence.hpp"

namespace rgx {

    Sequence::Sequence(std::string const &seq, int min, int max):seq(seq) {
        this->min = min;
        this->max = (max == -1 ? min : max);
    }

    bool Sequence::parse(std::string const &str, size_t &idx) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (seq.compare(0, seq.size(), str) != 0)
                return false;
            idx += seq.size();
        }
        return true;
    }
}