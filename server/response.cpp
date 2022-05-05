#include "./response.hpp"
#include "./request.hpp"
#include <sstream>

response::response() {}
response::response(IParseable &header, IParseable &body, bool bad_req) : pos(0)
{
    check_errors(header, body, bad_req);
}

response::~response() {}

size_t response::read(char *buff, size_t size)
{
    size_t ret = body.read(buff, size).gcount();
    return ret;
}

void response::check_errors(IParseable &header, IParseable &body, bool bad_req)
{
    if (bad_req)
        generate_response_error(400, "Bad Request");
    else if (header[0]["uri"].get_string().size() >= 2048)
        generate_response_error(414, "Request URI too long");
    else if (header[0]["version"].get_string() != "HTTP/1.1")
        generate_response_error(505, "HTTP version not supported");
    else
        generate_response_error(404, "OK");
}

void response::generate_response_error(short error, std::string const &message)
{
    std::string body = "<div class='main-error-page'>";
    body += "<center>";
    body += "<h1 class='error-title'>";
    body += std::to_string(error) + "  " + message;
    body += " </h1>";
    body += "<h2 class='error-subtitle'>";
    body += "sir lah yn... sir fhalk";
    body += "</h2>";
    body += " </center>";
    body += "</div>";

    header = "HTTP/1.1 " + std::to_string(error) + " OK" + "\n\r";
    header += "content-length: " + std::to_string(body.size()) + "\r\n" + "\r\n";

    this->body.clear();
    this->body << header;
    this->body << body;
}

std::string const &response::get_header()
{
    return header;
}