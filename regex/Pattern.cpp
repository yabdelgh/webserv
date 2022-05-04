#include "./Pattern.hpp"

namespace rgx {

    Pattern::Pattern() {}

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

    Pattern &Pattern::append(Pattern const &pattern)
    {
        for (size_t i = 0; i < pattern.tokens.size(); i++)
        {
            tokens.push_back(pattern.tokens[i]->clone());
        }
        return *this;
    }

    bool Pattern::find(std::string const &str) {
        stringstream ss;
        idx = 0;
        return find(str, idx);
    }

    bool Pattern::find(std::string const &str, size_t &idx) {
        stringstream ss;
        size_t i;

        content = "";
        this->idx = idx;
        for (i = 0; i < tokens.size(); i++)
        {
            if (tokens[i]->find(str, this->idx, ss) == false)
            {
                reached_end = tokens[i]->is_reached_end();
                return false;
            }
        }
        if (!tokens.empty())
            reached_end = tokens[tokens.size() - 1]->is_reached_end();
        idx = this->idx;
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

    bool Pattern::is_reached_end() const
    {
        return reached_end;
    }

    string const &Pattern::get_content() {
        return content;
    }
}