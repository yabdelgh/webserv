#include "./request.hpp"
#include "tools.hpp"
#include <iostream>


request:: request(/* args */):header(*get_request_header()),body(*get_request_header())
{
    resp = nullptr;
    status = INCOMPLETE_HEADER;
    resp_status = http::OK;
    lenght_pattern = GS.patterns["[a-fA-F0-9]+\r\n"];
    body_size = 0;
    remainder_body_size = 0;
    strcpy(body_filename, "/tmp/webserv_XXXXXXXXXXXXXXX");
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
            status = REQUEST_READY;
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
                status = REQUEST_READY;
                resp_status = http::BAD_REQUEST;
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
        else if (header[0]["method"].get_string() == "DELETE")
            status = REQUEST_READY;
        else if (header[0]["method"].get_string() == "POST")
        {
            if (body_stream && body_stream->is_open())
                write_body();
            else if (header[1].contains("Content-Length"))
            {
                // get the exact config for this request after parsing the header
                chunked = false;
                body_size  = remainder_body_size = header[1]["Content-Length"].num();
                if (body_size > body_size_limit)
                {
                    status = REQUEST_READY;
                    resp_status = http::PAYLOAD_TOO_LARGE;
                }
                body_stream = new std::ofstream(mktemp(body_filename), std::ios::trunc);
            }
            else if (header[1].contains("Transfer-Encoding") && header[1]["Transfer-Encoding"].str() == "chunked")
            {
                chunked = true;
                body_stream = new std::ofstream(mktemp(body_filename), std::ios::trunc);
            }
            else 
            {
                status = REQUEST_READY;
                resp_status = http::LENGTH_REQUIRED;
            }
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
    resp = new response(header, body, resp_status);
    status = INCOMPLETE_HEADER;
    return *resp;
}

void request::reset()
{
    body_size = 0;
    remainder_body_size = 0;
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

void request::write_body()
{
    if (chunked)
    {
        if (lenght_pattern.find(content))
        {
            std::string const & str_size = lenght_pattern().get_content();
            size_t size = std::stol(str_size, nullptr, 16);
            if (size + str_size.size() >= content.size())
            {
                body_stream->write(content.c_str() + str_size.size(), size);
                content  = &content[size + str_size.size()];
                body_size += size;
            }
            if (size == 0)
                status = REQUEST_READY;
        }
        if (body_size > body_size_limit)
        {
            status = REQUEST_READY;
            resp_status = http::PAYLOAD_TOO_LARGE;
        }
    }
    else
    {
        size_t size = std::min(remainder_body_size, content.size());
        body_stream->write(content.c_str(), size);
        remainder_body_size -= size;
        content  = &content[size];
        if (remainder_body_size = 0)
            status = REQUEST_READY;
    }
}