#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "tools.hpp"
#include "IParseable.hpp"
#include "response.hpp"
#include <list>

enum RequestStatus{
    REQUEST_READY,
    HEADER_READY,
    INCOMPLETE_HEADER,
    INCOMPLETE_BODY,
    BAD_REQUEST
};

class  request
{
private:
    IParseable *header;
    IParseable *body;
    RequestStatus status;
    std::string content;
    size_t content_size;
    std::list<response> responses;

private:
    void generate_response();

public:
    request(/* args */);
    ~ request();

    RequestStatus get_status() const;
    void append_data(char const *data);
    std::string &get_remainder();
    void parse_header();
    void parse_body();
    std::list<response> pop_responses();
};

#endif