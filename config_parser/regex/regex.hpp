#include <vector>
#include "./patterns/APattern.hpp"

namespace rgx
{
    class Regex
    {
    private:
        std::vector<IPattern> patterns;
        size_t idx;
    public:
        Regex();
        ~Regex();
        Regex &append(IPattern const &pattern);
        bool parse(std::string const &str);
    };
}
