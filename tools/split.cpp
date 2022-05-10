#include <vector>
#include "tools.hpp"

std::vector<std::string> split(std::string const & str, std::string const &delm)
{
    size_t start = 0;
    size_t end = 0;
    std::vector<std::string> arr;

    while(start != -1)
    {
        if ((end = str.find_first_of(delm, start)) == -1)
        {
            arr.push_back(str.substr(start));
            break;
        }
        arr.push_back(str.substr(start, end - start));
            start = std::min(str.size(), str.find_first_not_of(delm, end));
    }
    return arr;
}
