#include "./request.hpp"


request:: request(/* args */)
{
}

request::~ request()
{
}

void request::parse_header()
{
    if (status == INCOMPLETE_HEADER)
    {
        size_t idx;
        bool parse_res = header->parse(content, idx);
        content = &content[idx];
        if (!parse_res && !header->is_reached_end())
            status = BAD_REQUEST;
        else if (parse_res)
        {
            status = INCOMPLETE_BODY;
            parse_body();
        }
    }
}

void request::parse_body()
{
    if (status == INCOMPLETE_BODY)
    {
        size_t idx;
        bool parse_res = body->parse(content, idx);
        content = &content[idx];
        if (!parse_res && !body->is_reached_end())
            status = BAD_REQUEST;
        else if (parse_res)
        {
            status = REQUEST_READY;
        }
    }
}

void request::append_data(char const * data)
{
    content += data;
    size_t idx;
    do
    {
        parse_header();
        parse_body();
        generate_response();
    }while (status == REQUEST_READY);
    content = &content[idx];
}

std::string &request::get_remainder()
{
    return content;
}

RequestStatus request::get_status() const
{
    return status;
}