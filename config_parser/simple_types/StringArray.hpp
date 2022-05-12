#ifndef STRING_ARRAY_HPP
#define STRING_ARRAY_HPP

#include "./ASimpleType.hpp"
#include "tools.hpp"
#include <set>

class StringArray: public ASimpleType
{
private:
    std::vector<std::string> value;
    std::set<std::string> set;
    std::string delimiter;

protected:
    void set_value(std::string const &raw);

public:
    StringArray(rgx::Pattern const &pattern, std::string delimiter = WHITESPACE);
    StringArray(rgx::Pattern const &pattern, std::string default_val[], size_t size, std::string delimiter = WHITESPACE);
    StringArray(StringArray const &other);
    ~StringArray();
    
    StringArray &operator=(StringArray const &other);
    std::vector<std::string> &get_str_array();
    std::set<std::string> &str_set();

    IParseable *clone() const;
};

#endif