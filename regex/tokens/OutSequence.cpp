#include "./OutSequence.hpp"

namespace rgx {

    OutSequence::OutSequence(std::string const &seq, int min, int max):AToken(min, max), seq(seq) {
    }

    OutSequence::OutSequence(OutSequence const &other) {
        *this = other;
    }

    OutSequence &OutSequence::operator=(OutSequence const &other) {
        if (this != &other) {
            this->seq = other.seq;
            AToken::operator=(other);
        }
        return *this;
    }

    OutSequence::~OutSequence() {}

    bool OutSequence::find(std::string const &str, size_t &idx, stringstream &ss) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            if (seq.compare(0, seq.size(), str) == 0)
                break;
            ss << seq;
            idx += seq.size();
        }
        return is_matched(i);
    }

    bool OutSequence::match(string const &str, size_t &idx) {
        size_t i;
        for (i = 0; get_more(i) ; i++)
        {
            if (seq.compare(0, seq.size(), str) == 0)
                break;
            idx += seq.size();
        }
        return is_matched(i);
    }

    AToken *OutSequence::clone() const {
        return (new OutSequence(*this));
    }
}