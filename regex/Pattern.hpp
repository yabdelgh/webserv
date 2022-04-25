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
    public:
        Pattern();
        ~Pattern();
        Pattern &append(AToken const &pattern);
        bool find(std::string const &str);
        bool find(std::string const &str, size_t &idx);
        bool match(std::string const &str);
        string const &get_content();
    };
}