#ifndef INT_ARRAY_HPP
#define INT_ARRAY_HPP

#include "./ASimpleType.hpp"

class IntArray: public ASimpleType
{
private:
    std::vector<int> value;
    std::set<int> set;

protected:
    void set_value(std::string const &raw);

public:
    IntArray(rgx::Pattern const &pattern);
    IntArray(rgx::Pattern const &pattern, std::vector<int> const& default_val);
    IntArray(IntArray const &other);
    ~IntArray();
    IntArray &operator=(IntArray const &other);
    std::vector<int> &get_int_array();
    std::set<int> &int_set();
    IParseable *clone() const;
};

#endif