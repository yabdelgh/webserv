#include <vector>
#include "./patterns/APattern.hpp"

namespace rgx
{
    class Regex
    {
    private:
        std::vector<APattern> patterns;
        size_t idx;
    public:
        Regex();
        ~Regex();
        Regex &append(APattern const &pattern);
        bool parse(std::string const &str);
    };
}
