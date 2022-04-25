#ifndef STRING_HPP
#define STRING_HPP

#include "./ASimpleType.hpp"

class String:public ASimpleType
{
private:
    std::string value;

protected:
    void set_value(std::string const &raw);

public:
    String(rgx::Pattern const &pattern);
    String(String const &other);
    ~String();
    String &operator=(String const &other);
    std::string &get_string();
    IParseable *clone() const;
};

#endif