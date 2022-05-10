#include "./request.hpp"
#include "tools.hpp"
#include <iostream>


request:: request(/* args */):header(*get_request_header()),body(*get_request_header())
{
    resp = nullptr;
    status = INCOMPLETE_HEADER;
}

request::~ request()
{
}

void request::parse_header()
{
    size_t idx = 0;
    if (status == INCOMPLETE_HEADER)
    {
        std::cout << "content: " << content << std::endl;
        bool parse_res = header.cont_parse(content, idx);
        content = &content[idx];
        std::cout << "remainder: " << content << std::endl;
        if (!parse_res && !header.is_reached_end())
        {
            std::cout << "bad header" << std::endl;
            // status = BAD_REQUEST;
            // generate_response();
        }
        else if (parse_res)
        {
            std::cout << "header complited" << std::endl;
            status = INCOMPLETE_BODY;
        }
        else
            std::cout << "incomplete_header" << std::endl;
    }
}

void request::parse_header(std::string const &data)
{
    size_t idx = 0;
    if (status == INCOMPLETE_HEADER)
    {
        if (content.find("\r\n\r\n") != -1)
        {
            if (header.parse(content, idx) == false)
            {
                std::cout << "bad header" << std::endl;
                status = BAD_REQUEST;
                return;
            }
            content = &content[idx];
            std::cout << "remainder |" << content << "|" << std::endl;
            status = INCOMPLETE_BODY;
        }
    }
}

void request::parse_body()
{
    size_t idx = 0;
    if (status == INCOMPLETE_BODY)
    {   
        if (header[0]["method"].get_string() == "GET")
            status = REQUEST_READY;
        else
        {
            bool parse_res = body.cont_parse(content, idx);
            content = &content[idx];
            // if (!parse_res && !body.is_reached_end())
            //     status = BAD_REQUEST;
            // else if (parse_res)
            // {
            //     status = REQUEST_READY;
            // }
        }
    }
}

void request::append_data(char const * data)
{
	std::cout << "appending" << std::endl;
    content += data;

    // std::cout << "parse_header" << std::endl;
    // parse_header(data);
    // std::cout << "parse_body" << std::endl;
    // parse_body();
    // std::cout << "generate_response" << std::endl;
    // generate_response();
}

void request::handle()
{
    parse_header("");
    parse_body();
}

response &request::gen_response()
{
    delete resp;
    resp = new response(header, body, status == BAD_REQUEST);
    status = INCOMPLETE_HEADER;
    return *resp;
}

void request::reset()
{
    content_size = 0;
    content = "";
    status = INCOMPLETE_HEADER;
}

std::string &request::get_remainder()
{
    return content;
}

RequestStatus request::get_status() const
{
    return status;
}

void request::set_status(RequestStatus status)
{
    this->status = status;
}