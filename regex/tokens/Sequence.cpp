#include "./Sequence.hpp"

namespace rgx {

    Sequence::Sequence(std::string const &seq, int min, int max):AToken(min, max), seq(seq) {
    }

    Sequence::Sequence(Sequence const &other) {
        *this = other;
    }

    Sequence &Sequence::operator=(Sequence const &other) {
        if (this != &other) {
            this->seq = other.seq;
            AToken::operator=(other);
        }
        return *this;
    }

    Sequence::~Sequence() {}

    bool Sequence::find(std::string const &str, size_t &idx, stringstream &ss) {
        size_t i;
        size_t tmp_idx = idx;
        for (i = 0; get_more(i) ; i++)
        {
            if (seq.compare(0, str.size(), &str[tmp_idx], seq.size()) != 0)
                break;
            ss << seq;
            tmp_idx += seq.size();
        }
        if (is_matched(i))
        {
            idx = tmp_idx;
            return true;
        }
        return false;
    }

    bool Sequence::match(string const &str, size_t &idx) {
        size_t i;
        size_t tmp_idx = idx;
        for (i = 0; get_more(i) ; i++)
        {
            if (seq.compare(0, seq.size(), str) != 0)
                break;
            tmp_idx += seq.size();
        }
        if (is_matched(i))
        {
            idx = tmp_idx;
            return true;
        }
        return false;
    }

    AToken *Sequence::clone() const {
        return (new Sequence(*this));
    }
}