#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <list>
#include "tools.hpp"
#include "IParseable.hpp"
#include "response.hpp"
#include "enum.hpp"

class  request
{
private:
    IParseable &header;
    IParseable &body;
    RequestStatus status;
    std::string content;
    size_t content_size;
    std::list<response> responses;
    response *resp;

public:
    request(/* args */);
    ~ request();

    RequestStatus get_status() const;
    void append_data(char const *data);
    void handle();
    std::string &get_remainder();
    void parse_header();
    void parse_header(std::string const &data);
    void parse_body();
    response &get_response();
    void set_status(RequestStatus status); // temporary
    void reset();
};

#endif