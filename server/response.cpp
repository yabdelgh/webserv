#include "./response.hpp"
#include "./request.hpp"
#include <sstream>

response::response(IParseable &header, IParseable &body, int status):pos(0)
{
    status = 0;
}

response::~response() {}

size_t response::read(char *buff, size_t size)
{
    std::stringstream ss;
    ss << "HTTP/1.1 200 OK\r\n";
    ss << "Content-Type: text/html; charset=UTF-8\r\n\r\n";         
    ss << "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>";
    ss << "<style>body { background-color: #111 }";
    ss << "h1 { font-size:4cm; text-align: center; color: black;";
    ss << " text-shadow: 0 0 2mm red}</style></head>";
    ss << "<body><h1>Goodbye, world!</h1></body></html>\r\n";


    if (status == BAD_REQUEST)
    {
        if (pos == 0)
        {
            ss << "HTTP/1.1 200 OK\r\n";
            ss << "Content-Type: text/html; charset=UTF-8\r\n\r\n";         
            ss << "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>";
            ss << "<style>body { background-color: #111 }";
            ss << "h1 { font-size:4cm; text-align: center; color: black;";
            ss << " text-shadow: 0 0 2mm red}</style></head>";
            ss << "<body><h1>Bad, Request!</h1></body></html>\r\n";
            strcpy(buff, ss.str().data());
            return ss.str().size();
        }
    }
    else
    {
        if (pos == 0)
        {
            std::stringstream ss;
            ss << "HTTP/1.1 200 OK\r\n";
            ss << "Content-Type: text/html; charset=UTF-8\r\n\r\n";         
            ss << "<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>";
            ss << "<style>body { background-color: #111 }";
            ss << "h1 { font-size:4cm; text-align: center; color: black;";
            ss << " text-shadow: 0 0 2mm red}</style></head>";
            ss << "<body><h1>Goodbye, world!</h1></body></html>\r\n";
            strcpy(buff, ss.str().data());
            return ss.str().size();
        }
    }
    return 0;
}