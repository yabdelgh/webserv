#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "IParseable.hpp"
#include <sstream>
#include "enum.hpp"

class response
{
private:
    size_t pos;
    short status;
    std::string header;
    OutPutType input_type;
    std::stringstream body;

public:
    response();
    response(IParseable &header, IParseable &body, bool bad_req);
    ~response();
    // handle_get();
    void check_errors(IParseable &header, IParseable &body, bool bad_req);
    void make_header();
    size_t read(char *buff, size_t size);
    std::string const &get_header();
    void generate_response_error(short error, std::string const &message);
};

#endif