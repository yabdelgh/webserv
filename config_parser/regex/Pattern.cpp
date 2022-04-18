#include "./Pattern.hpp"

namespace rgx {

    Pattern::Pattern():idx(0) {}

    Pattern::Pattern(string const &ptrn):idx(0) {
        vector<string> tokens;
        size_t idx = 0;
        while (idx < ptrn.size())
        {
            size_t start = ptrn.find('[', idx);
            if (start == -1)
                throw runtime_error("invalid pattern at " + to_string(idx));

            size_t end = ptrn.find(']', idx);
            end = (end == -1 ? ptrn.size() - 1 : end);
            idx = end + 1;
            tokens.push_back(ptrn.substr(start + 1, end - (start + 1))); // call token factory
        }
    }

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