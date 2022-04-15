#ifndef IPATTERN_HPP
#define IPATTERN_HPP

# include <string>


namespace rgx {
    class IPattern
    {
    protected:
        int min;
        int max;
        std::string content;

    public:
        IPattern(/* args */);
        virtual ~IPattern();
        virtual bool parse(std::string const &str, size_t &idx) = 0;

    protected:
        bool get_more(size_t idx);
    };
}
#endif