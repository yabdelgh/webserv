#ifndef IPARSEABLE_HPP
#define IPARSEABLE_HPP

#include<string>
#include<vector>
#include<set>
#include<cstring>

#define INVALID_TYPE_ERROR "INVALID TYPE"

class IParseable
{
private:
    /* data */
public:
    virtual int get_int() = 0;
    virtual int num() = 0;
    virtual double get_double() = 0;
    virtual double dbl() = 0;
    virtual std::string &get_string() = 0;
    virtual std::string &str() = 0;
    virtual std::vector<std::string> &get_str_array() = 0;
    virtual std::vector<std::string> &str_arr() = 0;
    virtual std::vector<int> &get_int_array() = 0;
    virtual std::vector<int> &int_arr() = 0;
    virtual std::vector<double> &get_double_array() = 0;
    virtual std::vector<double> &dbl_arr() = 0;
    virtual std::set<int> &int_set() = 0;
    virtual std::set<std::string> &str_set() = 0;
    virtual std::set<double> &dbl_set() = 0;

    virtual IParseable &operator[](std::string const &) = 0;
    virtual IParseable &operator[](size_t idx) = 0;
    virtual size_t size() const = 0;
    virtual IParseable *clone() const = 0;
    virtual bool parse(std::string const &str, size_t &idx) = 0;
    virtual bool parse(std::vector<char> str, size_t &idx) = 0;
    virtual bool cont_parse(std::string &str, size_t &idx) = 0;
    virtual bool is_reached_end() const = 0;
    virtual bool is_parsed() const = 0; 
    virtual bool contains(std::string const &name) = 0;
    virtual ~IParseable() {};
};

#endif