#ifndef APATTERN_HPP
#define APATTERN_HPP

# include <string>
# include <sstream>


namespace rgx {
    using namespace std;

    class APattern
    {
    protected:
        int min;
        int max;
        string content;

    public:
        APattern(int min = - 1, int max = -1);
        virtual ~APattern();
        virtual bool parse(string const&str, size_t &idx) = 0;
        virtual bool parse(string const&str, size_t &idx, stringstream &ss) = 0;

    protected:
        bool get_more(size_t idx);
        bool is_valid_parse(size_t times_count);
    };
}
#endif