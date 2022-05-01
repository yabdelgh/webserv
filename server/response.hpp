#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "IParseable.hpp"

class response
{
private:
    size_t pos;
    int status;

public:
    response(IParseable &header, IParseable &body, int status);
    ~response();
    size_t read(char *buff, size_t size);
};

#endif