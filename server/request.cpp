#include <iostream>
#include <string.h>
#include "./request.hpp"
#include "tools.hpp"
#include "server_config_helper.hpp"
#include "GlobalStorage.hpp"


request:: request(/* args */)
{
    header = nullptr;
    socket_id = -1;
    resp = nullptr;
    body_stream = nullptr;
    status = INCOMPLETE_HEADER;
    resp_status = http::OK;
    lenght_pattern = GS.patterns["[a-fA-F0-9]+\r\n"];
    body_size = 0;
    remainder_body_size = 0;
    body_size_limit = 0;
    strcpy(body_filename, "/tmp/webserv_XXXXXXXXXXXXXXX");
}

request:: request(int socket_id)
{
    header = nullptr;
    this->socket_id = socket_id;
    resp = nullptr;
    body_stream = nullptr;
    status = INCOMPLETE_HEADER;
    resp_status = http::OK;
    lenght_pattern = GS.patterns["[a-fA-F0-9]+\r\n"];
    body_size = 0;
    remainder_body_size = 0;
    body_size_limit = 0;
    strcpy(body_filename, "/tmp/webserv_XXXXXXXXXXXXXXX");
}

request::request(request const& other)
{
    header = nullptr;
    resp = nullptr;
    body_stream = nullptr;
    *this = other;
}

request::~ request()
{
}

request &request::operator=(request const& other)
{
    if (this != &other)
    {
        this->socket_id = other.socket_id;
        this->chunked = other.chunked;
        this->body_size = other.body_size;
        this->remainder_body_size = other.remainder_body_size;
        this->body_size_limit = other.body_size_limit;
        this->body_stream = other.body_stream;
        // this->header = other.header; reference
        // this->body = other.body; reference
        this->status = other.status;
        this->resp_status = other.resp_status;
        this->content = other.content;
        this->lenght_pattern = other.lenght_pattern;
        strcpy(this->body_filename, other.body_filename);
        this->req_conf = other.req_conf;
        this->loc_conf = other.loc_conf;
        this->resp = other.resp;
        this->body_stream = other.body_stream;
    }
    return *this;
}

// void request::parse_header()
// {
//     size_t idx = 0;
//     if (status == INCOMPLETE_HEADER)
//     {
//         std::cout << "content: " << content.data() << std::endl;
//         // bool parse_res = header.cont_parse(content, idx);
//         // content = &content[idx];
//         content = std::vector<std::string>(&content[idx], &content.back());
//         std::cout << "remainder: " << content.data() << std::endl;
//         if (!parse_res && !header.is_reached_end())
//         {
//             std::cout << "bad header" << std::endl;
//             status = REQUEST_READY;
//         }
//         else if (parse_res)
//         {
//             std::cout << "header complited" << std::endl;
//             status = INCOMPLETE_BODY;
//         }
//         else
//             std::cout << "incomplete_header" << std::endl;
//     }
// }

void request::parse_header()
{
    size_t idx = 0;
    if (status == HEADER_READY)
    {
        if (!header->parse(content, idx) || !(*header)[1].contains("host"))
        {
            std::cout << "bad header" << std::endl;
            status = REQUEST_READY;
            resp_status = http::BAD_REQUEST;
            req_conf = find_server_conf(GS.socket_confs[socket_id], "");
            return;
        }
        req_conf = find_server_conf(GS.socket_confs[socket_id], (*header)[1]["host"].str());
        loc_conf = find_location((*req_conf)["location"], (*header)[0]["uri"].str());
        content = std::vector<char>(&content[idx], &content[content.size()]);
        status = INCOMPLETE_BODY;
    }
}

void request::parse_body()
{
    size_t idx = 0;
    if (status == INCOMPLETE_BODY)
    {  
        if ((*header)[0]["method"].get_string() == "GET")
            status = REQUEST_READY;
        else if ((*header)[0]["method"].get_string() == "DELETE")
            status = REQUEST_READY;
        else if ((*header)[0]["method"].get_string() == "POST")
        {
            if ((*header)[1].contains("Content-Length"))
            {
                (*header)[1]["Content-Length"].num();
                body_size_limit = get_client_body_limit(*req_conf, loc_conf);
                chunked = false;
                body_size  = remainder_body_size = (*header)[1]["Content-Length"].num();
                std::cout << "content_length: " << body_size << "limit: " << body_size_limit << std::endl;
                if (body_size > body_size_limit)
                {
                    status = REQUEST_READY;
                    resp_status = http::PAYLOAD_TOO_LARGE;
                }
                status = SAVING_BODY;
                body_stream = new std::ofstream(mktemp(body_filename), std::ios::trunc);
            }
            else if ((*header)[1].contains("Transfer-Encoding") && (*header)[1]["Transfer-Encoding"].str() == "chunked")
            {
                std::cout << "body chunked" << std::endl;
                chunked = true;
                status = SAVING_BODY;
                body_stream = new std::ofstream(mktemp(body_filename), std::ios::trunc);
            }
            else 
            {
                status = REQUEST_READY;
                resp_status = http::LENGTH_REQUIRED;
            }
        }
    }
    if (status == SAVING_BODY)
        write_body();
}

// void request::append_data(char const * data)
// {
// 	std::cout << "appending" << std::endl;
//     content += data;

//     // std::cout << "parse_header" << std::endl;
//     // parse_header(data);
//     // std::cout << "parse_body" << std::endl;
//     // parse_body();
//     // std::cout << "generate_response" << std::endl;
//     // generate_response();
// }

void request::append_data(char const * data, size_t size)
{
	std::cout << "appending" << std::endl;
    size_t n = content.size() - std::min(size_t(3), content.size());
    content.insert(content.end(), data, data + size);
    if (status == INCOMPLETE_HEADER && strnstr(content.data() + n, "\r\n\r\n", size + n))
        status = HEADER_READY;
}

void request::handle()
{
    if (socket_id < 0)
        throw std::runtime_error("request error : invalide socket id");
    if (header == nullptr)
        header = get_request_header();
    parse_header();
    parse_body();
}

response &request::gen_response()
{
    delete resp;
    resp = new response(*header, body_filename, req_conf, resp_status);
    delete header;
    header = nullptr;
    status = INCOMPLETE_HEADER;
    return *resp;
}

void request::reset()
{
    body_size = 0;
    remainder_body_size = 0;
    content.clear();
    status = INCOMPLETE_HEADER;
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
    std::cout << "body writer" << std::endl; 
    if (chunked)
    {
        if (lenght_pattern.find(content.data()))
        {
            std::string const & str_size = lenght_pattern.get_content();
            size_t size = std::stol(str_size, nullptr, 16);
            if (size + str_size.size() >= content.size())
            {
                // body.write(content.data() + str_size.size(), size);
                body_stream->write(content.data() + str_size.size(), size);
                // content = &content[size + str_size.size()];
                content = std::vector<char>(&content[size + str_size.size()], &content[content.size()]);
                body_size += size;
            }
            if (size == 0)
            {
                body_stream->flush();
                body_stream->close();
                status = REQUEST_READY;
                return;
            }
        }
        if (body_size > body_size_limit)
        {
            body_stream->close();
            status = REQUEST_READY;
            resp_status = http::PAYLOAD_TOO_LARGE;
        }
    }
    else
    {
        size_t size = std::min(remainder_body_size, content.size());
        // body.write(content.data(), size);
        body_stream->write(content.data(), size);
        remainder_body_size -= size;
        content = std::vector<char>(&content[size], &content[content.size()]);
        if (remainder_body_size == 0)
        {
            body_stream->flush();
            body_stream->close();
            status = REQUEST_READY;
            return;
        }
    }
    if (!body_stream->is_open() || body_stream->bad())
    {
        resp_status = http::INSUFFICIENT_STORAGE;
        status = REQUEST_READY;
    }
    // std::cout << "body: |" << body.str() << "|" << std::endl; 
}