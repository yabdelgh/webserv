#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <list>
#include <fstream>
#include <sstream>
#include "tools.hpp"
#include "IParseable.hpp"
#include "Pattern.hpp"
#include "response.hpp"
#include "enum.hpp"
#include "GlobalStorage.hpp"
#include <cstdio>

class  request
{
private:
    bool chunked;
    int socket_id;
    size_t body_size;
    size_t remainder_body_size;
    size_t body_size_limit;
    std::ofstream *body_stream;
    IParseable &header;
    std::stringstream body;
    RequestStatus status;
    http::HttpStatusCode resp_status;
    // std::string content;
    std::vector<char> content;
    rgx::Pattern lenght_pattern;
    char body_filename[30];
    IParseable *req_conf;
    IParseable *loc_conf;

public:
    response *resp;

private:
    void write_body();

public:
    request(/* args */);
    request(int socket_id);
    request(request const& other);
    request &operator=(request const& other);
    ~ request();
    RequestStatus get_status() const;
    // void append_data(char const *data);
    void append_data(char const *data, size_t size);
    void handle();
    // void parse_header();
    void parse_header();
    void parse_body();
    response &gen_response();
    void set_status(RequestStatus status); // temporary
    void reset();
};

#endif