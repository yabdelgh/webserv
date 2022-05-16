#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "IParseable.hpp"
#include <sstream>
#include "enum.hpp"

int launch_cgi(IParseable &header, IParseable &loc, std::string &filename, const char *path);

class response
{
private:
    size_t pos;
    short status;
    bool finished;
    bool header_finished;
    bool content_len; // flag for know when content-length is present in the response
    int fd; // fd to read the request from cgi
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
    response(IParseable &rheader, std::string rbody, IParseable *sconf, short status);
    ~response();
    // handle_get();
    bool request_valide(IParseable &header);
    size_t read_body(char *buff, size_t size);
    size_t read_header(char *buff, size_t size);
    char *get_header();
    short get_status();
    bool is_finished();
    bool is_header_finished();

    void generate_response_error(short error, std::string const &message = "");
    void generate_redirect(int status, std::string const& location);
    void generate_autoindex(std::string const&path);
    void prepare_body(std::string const& path);

    std::string contentType(std::string path);
    void set_header(std::string const& name, std::string const& value);

    void handle_cgi_req(IParseable &rheader, std::string &rbody);
    void handle_get_req(IParseable &rheader, std::string &rbody);
    void handle_get_delete(IParseable &rheader, std::string &rbody);
    bool extract_config(IParseable &req_header);
    bool join_index(std::string &path);
};

#endif