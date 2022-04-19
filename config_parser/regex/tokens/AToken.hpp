#ifndef ATOKEN_HPP
#define ATOKEN_HPP

# include <string>
# include <sstream>

#define between(x, min, max) (x >= min && x<= max)

namespace rgx {
    using namespace std;

    class AToken
    {
    protected:
        static unordered_map<string, struct Pattern> parse_patterns;
        int min;
        int max;
        string content;

    public:
        AToken(int min = - 1, int max = -1);
        virtual ~AToken();
        virtual bool parse(string const&str, size_t &idx);
        virtual bool parse(string const&str, size_t &idx, stringstream &ss) = 0;
        virtual AToken *clone() const = 0;

    protected:
        bool get_more(size_t idx);
        bool is_matched(size_t times_count);
    };
}
#endif