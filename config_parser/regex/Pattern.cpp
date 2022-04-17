#include "./Pattern.hpp"

namespace rgx {

    Pattern::Pattern():idx(0) {}
    Pattern::Pattern(string const &ptrn):idx(0) {}

    Pattern::~Pattern() {}

    Pattern &Pattern::append(AToken const &pattern) {
        patterns.push_back(pattern);
    }

    bool Pattern::find(std::string const &str) {
        stringstream ss;
        for (size_t i = 0; i < patterns.size(); i++)
        {
            if (patterns[i].parse(str, idx, ss) == false)
                return false;
        }
        return true;
    }

    bool Pattern::match(std::string const &str) {
        for (size_t i = 0; i < patterns.size(); i++)
        {
            if (patterns[i].parse(str, idx) == false)
                return false;
        }
        return true;
    }
}