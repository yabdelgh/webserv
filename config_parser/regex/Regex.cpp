#include "./Regex.hpp"

namespace rgx {

    Regex::Regex():idx(0) {}

    Regex::~Regex() {}

    Regex &Regex::append(APattern const &pattern) {
        patterns.push_back(pattern);
    }

    bool Regex::parse(std::string const &str) {
        for (size_t i = 0; i < patterns.size(); i++)
        {
            if (patterns[i].parse(str, idx) == false)
                return false;
        }
        return true;
    }
}