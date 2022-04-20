#include "./Sequence.hpp"

namespace rgx {

    Sequence::Sequence(std::string const &seq, int min, int max):seq(seq) {
        this->min = min;
        this->max = (max == -1 ? min : max);
    }

    Sequence::Sequence(string const &ptrn, size_t &idx) {
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

    bool Sequence::find(std::string const &str, size_t &idx, stringstream &ss) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (seq.compare(0, seq.size(), str) != 0)
                break;
            ss << seq;
            idx += seq.size();
        }
        return is_matched(idx);
    }

    bool Sequence::match(string const &str, size_t &idx) {
        for (size_t i = 0; get_more(i) ; i++)
        {
            if (seq.compare(0, seq.size(), str) != 0)
                break;
            idx += seq.size();
        }
        return is_matched(idx);
    }

    AToken *Sequence::clone() const {
        return (new Sequence());
    }
}