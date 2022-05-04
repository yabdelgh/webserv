#include "./request.hpp"
#include "tools.hpp"
#include <iostream>


request:: request(/* args */)
{
    header = get_request_header();
    status = INCOMPLETE_HEADER;
    responses.clear();
    body = get_request_header();
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
        bool parse_res = header->cont_parse(content, idx);
        content = &content[idx];
        std::cout << "remainder: " << content << std::endl;
        if (!parse_res && !header->is_reached_end())
        {
            std::cout << "bad header" << std::endl;
            status = BAD_REQUEST;
            generate_response();
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

void request::parse_body()
{
    size_t idx = 0;
    if (status == INCOMPLETE_BODY)
    {
        if (header[0]["method"].get_string() == "GET")
            status = REQUEST_READY;
        else
        {
            bool parse_res = body->cont_parse(content, idx);
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

    std::cout << "parse_header" << std::endl;
    parse_header();
    std::cout << "parse_body" << std::endl;
    parse_body();
    std::cout << "generate_response" << std::endl;
    generate_response();
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
    return responses;
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