#include "./response.hpp"
#include "./request.hpp"

response::response(IParseable &header, IParseable &body, int status):pos(0)
{
    status = 0;
}

response::~response() {}

size_t response::read(char *buff, size_t size)
{
    if (status == BAD_REQUEST)
    {
        if (pos == 0)
        {
            strcpy(buff, "bad request");
            return 11;
        }
    }
    else
    {
        if (pos == 0)
        {
            strcpy(buff, "1234567890");
            return 10;
        }
    }
    return 0;
}