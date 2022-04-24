#include "./Pattern.hpp"

namespace rgx {

    Pattern::Pattern():idx(0) {}

    // Pattern::Pattern(string const &ptrn):idx(0) {
    //     vector<string> tokens;
    //     size_t idx = 0;
    //     while (idx < ptrn.size())
    //     {
    //         size_t start = ptrn.find('[', idx);
    //         if (start == -1)
    //             throw runtime_error("invalid pattern at " + to_string(idx));

    //         size_t end = ptrn.find(']', idx);
    //         end = (end == -1 ? ptrn.size() - 1 : end);
    //         idx = end + 1;
    //         tokens.push_back(ptrn.substr(start + 1, end - (start + 1))); // call token factory
    //     }
    // }

    Pattern::~Pattern() {}

    Pattern &Pattern::append(AToken const &token) {
        tokens.push_back(token.clone());
        return *this;
    }

    bool Pattern::find(std::string const &str) {
        stringstream ss;
        idx = 0;
        find(str, idx);
    }

    bool Pattern::find(std::string const &str, size_t &idx) {
        stringstream ss;
        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens[i]->find(str, idx, ss) == false)
                return false;
        }
        content = ss.str();
        return true;
    }

    bool Pattern::match(std::string const &str) {
        idx = 0;
        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens[i]->match(str, idx) == false)
                return false;
        }
        return idx == str.size();
    }

    string const &Pattern::get_content() {
        return content;
    }
}