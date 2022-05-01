#include "./request.hpp"
#include "tools.hpp"
#include <iostream>


request:: request(/* args */)
{
    header = get_request_header();
    status = INCOMPLETE_HEADER;
    // body = get_request_header();
}

request::~ request()
{
}

void request::parse_header()
{
    if (status == INCOMPLETE_HEADER)
    {
        size_t idx = 0;
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
        if (header[0]["method"].get_string() == "GET")
            status = REQUEST_READY;
        else
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
}

void request::append_data(char const * data)
{
	std::cout << "appending" << std::endl;
    content += data;
    do
    {
		std::cout << "generate_response" << std::endl;
        generate_response();
		std::cout << "parse_header" << std::endl;
        parse_header();
		std::cout << "parse_body" << std::endl;
        parse_body();
    }while (status == REQUEST_READY);
    // content = &content[idx];
}

std::string &request::get_remainder()
{
    return content;
}

RequestStatus request::get_status() const
{
    return status;
}

std::list<response> request::pop_responses() 
{
    std::list<response> tmp;
    tmp.assign(responses.begin(), responses.end());
    responses.clear();
    return tmp;
}

void request::generate_response()
{
    if (status == REQUEST_READY)
    {
        responses.push_back(response(*header, *body, status));
        status = INCOMPLETE_HEADER;
    }
    if (status == BAD_REQUEST)
    {
        responses.push_back(response(*header, *body, status));
    }
}