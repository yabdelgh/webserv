#ifndef INT_ARRAY_HPP
#define INT_ARRAY_HPP

#include "./ASimpleType.hpp"

class IntArray: public ASimpleType
{
private:
    std::vector<int> value;

protected:
    void set_value(std::string const &raw);

public:
    IntArray(rgx::Pattern const &pattern);
    IntArray(IntArray const &other);
    ~IntArray();
    IntArray &operator=(IntArray const &other);
    std::vector<int> &get_int_array();
    IParseable *clone() const;
};

#endif