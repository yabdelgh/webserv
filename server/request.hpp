#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <list>
#include <fstream>
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
    size_t body_size;
    size_t remainder_body_size;
    size_t body_size_limit;
    std::ofstream *body_stream;
    IParseable &header;
    IParseable &body;
    RequestStatus status;
    http::HttpStatusCode resp_status;
    std::string content;
    rgx::Pattern lenght_pattern;
    char body_filename[30];
    std::vector<IParseable *> *confs;
    IParseable *req_conf;
    IParseable *loc_conf;

private:
    void write_body();

public:
    response *resp;
    request(/* args */);
    request(request const& other);
    request &operator=(request const& other);
    ~ request();

    RequestStatus get_status() const;
    void append_data(char const *data);
    void handle();
    std::string &get_remainder();
    void parse_header();
    void parse_header(std::string const &data);
    void parse_body();
    response &gen_response();
    void set_status(RequestStatus status); // temporary
    void reset();
    void set_conf(std::vector<IParseable *> &confs);
};

#endif