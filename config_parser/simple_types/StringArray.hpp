#ifndef STRING_ARRAY_HPP
#define STRING_ARRAY_HPP

#include "./ASimpleType.hpp"

class StringArray: public ASimpleType
{
private:
    std::vector<std::string> value;
    std::string delimiter;

protected:
    void set_value(std::string const &raw);

public:
    StringArray(rgx::Pattern const &pattern);
    ~StringArray();
    std::vector<std::string> &get_str_array();

};

#endif