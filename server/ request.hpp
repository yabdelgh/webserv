#include "tools.hpp"
#include "IParseable.hpp"


enum RequestStatus{
    REQUEST_READY,
    INCOMPLETE_HEADER,
    INCOMPLETE_BODY,
};

class  request
{
private:
    IParseable *header;
    IParseable * body;
    RequestStatus status;
    std::string remainder;
    size_t content_size;

public:
    request(/* args */);
    ~ request();

    RequestStatus get_status();
    void append_data(char *data);
    std::string &get_remainder();
    void parse_header();
    void parse_body();
};

 request:: request(/* args */)
{
}

 request::~ request()
{
}
