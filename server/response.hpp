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
    bool finished;
    bool content_len;
    int fd;
    OutPutType input_type;
    char header_buff[4000];
    std::stringstream header;
    std::stringstream body;
    std::ifstream *bodyfile;

    IParseable *sconf;
    IParseable *loc;
    IParseable *error_pages;
    std::set<std::string> *allow_method;
    std::set<std::string> *indexs;
    std::string root;
    bool autoindex;
    bool redire;

public:
    response();
    response(IParseable &header, IParseable &body, short status);
    ~response();
    // handle_get();
    bool request_valide(IParseable &header, IParseable &body);
    size_t read(char *buff, size_t size);
    size_t read_header(char *buff, size_t size);
    char *get_header();
    bool is_finished();

    void generate_response_error(short error, std::string const &message = "");
    void generate_redirect(int status, std::string const& location);
    void generate_autoindex(std::string const&path);
    void prepare_body(std::string const& path);

    std::string contentType(std::string path);
    void set_header(std::string const& name, std::string const& value);

    void handle_get_req(IParseable &header, IParseable &body);
    void handle_post_req(IParseable &header, IParseable &body);
    void handle_get_delete(IParseable &header, IParseable &body);
    bool extract_config(IParseable &req_header);
    bool join_index(std::string &path);
};

#endif