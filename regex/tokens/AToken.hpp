#ifndef ATOKEN_HPP
#define ATOKEN_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <cstring>

#define between(x, min, max) (x >= min && x <= max)

namespace rgx {
    using namespace std;

    enum MatchDir {
        MATCH_IN,
        MATCH_OUT
    };

    class AToken
    {
    protected:
        int min;
        int max;
        size_t tmp_idx;
        string content;
        MatchDir match_dir;
        bool reached_end;

    public:
        AToken(int min = -1, int max = -1);
        AToken(MatchDir match_dir, int min = -1, int max = -1);
        AToken(AToken const &);
        AToken &operator=(AToken const &);
        virtual ~AToken();
        virtual bool match(string const&str, size_t &idx) = 0;
        virtual bool find(string const&str, size_t &idx);
        virtual bool find(string const&str, size_t &idx, stringstream &ss) = 0;
        virtual AToken *clone() const = 0;
        AToken &set_min(int min);
        AToken &set_max(int max);
        string const &get_content() const;
        bool is_reached_end() const;

    protected:
        bool get_more(size_t idx);
        bool is_matched(size_t times_count);
    };
}
#endif