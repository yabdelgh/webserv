#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <vector>
#include "./tokens/AToken.hpp"

namespace rgx
{
    class Pattern
    {
    private:
        std::vector<AToken *> tokens;
        size_t idx;
        string content;
        bool reached_end;
    public:
        Pattern();
        ~Pattern();
        Pattern &append(AToken const &pattern);
        bool find(std::string const &str);
        bool find(std::string const &str, size_t &idx);
        bool match(std::string const &str);
        bool is_reached_end() const;
        string const &get_content();
    };
}

#endif 