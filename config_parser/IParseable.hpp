#include<string>
#include<vector>

#define INVALID_TYPE_ERROR "INVALID TYPE"

class IParseable
{
private:
    /* data */
public:
    virtual int get_int();
    virtual double get_double();
    virtual std::string &get_string();
    virtual std::vector<std::string> &get_str_array();
    virtual std::vector<int> &get_int_array();
    virtual std::vector<double> &get_double_array();
    virtual IParseable &operator[](std::string);
    virtual IParseable &operator[](size_t idx);
    virtual size_t size() const;
    virtual IParseable *clone(bool deep = true) const;
    virtual bool parse(std::string &str, size_t &idx);
    virtual ~IParseable();
};
