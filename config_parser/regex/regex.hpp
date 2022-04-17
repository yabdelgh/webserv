#include <vector>
#include "./patterns/APattern.hpp"

namespace rgx
{
    class Regex
    {
    private:
        std::vector<AToken> patterns;
        size_t idx;
    public:
        Regex();
        ~Regex();
        Regex &append(AToken const &pattern);
        bool parse(std::string const &str);
    };
}
