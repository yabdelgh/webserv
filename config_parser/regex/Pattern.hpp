#include <vector>
#include "./tokens/AToken.hpp"

namespace rgx
{
    class Pattern
    {
    private:
        std::vector<AToken *> tokens;
        size_t idx;
    public:
        // Pattern(string const &ptrn);
        Pattern();
        ~Pattern();
        Pattern &append(AToken const &pattern);
        bool find(std::string const &str);
        bool match(std::string const &str);
    };
}