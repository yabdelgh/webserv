#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "tools.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <stdlib.h>
#include "./response.hpp"
#include "./request.hpp"
#include "server_config_helper.hpp"
#include "GlobalStorage.hpp"

response::response() {}
response::response(IParseable &rheader, std::string rbody, IParseable *sconf, short status)
{
    this->pos = 0;
    this->input_type = STREAM;
    this->sconf = sconf;
    this->loc = nullptr;
    this->error_pages = nullptr;
    this->allow_method = nullptr;
    this->indexs = nullptr;
    this->autoindex = false;
    this->redire = false;
    this->finished = false;
    this->header_finished = false;
    this->status = status;
    this->content_len = true;
    this->fd = -1;

    std::cout << "start response" << std::endl;
    if (extract_config(rheader))
        this->status = (status == http::OK ? http::BAD_REQUEST : status);
    std::cout << "config extraction done" << std::endl;
    if (!request_valide(rheader))
        return;
    std::cout << "method: " << rheader[0]["method"].str() << std::endl;
    if (redire)
    {
        IParseable &redire_inf = (*loc)[1]["return"];
        std::cout << "is redire " << redire_inf["status"].num() << "  |" <<redire_inf["uri"].str() << "|"<< std::endl;
        generate_redirect(redire_inf["status"].num(), redire_inf["uri"].str());
        return;
    }
    std::string ext = extension(rheader[0]["uri"].str());
    if (ext != "" && loc && (*loc)[1].contains("cgi_pass"))
        handle_cgi_req(rheader, rbody);
    else if (rheader[0]["method"].str() == "GET")
        handle_get_req(rheader, rbody);
    else if (rheader[0]["method"].str() == "DELETE")
        handle_get_delete(rheader, rbody);
    else
        generate_response_error(http::INTERNAL_SERVER_ERROR);
}

response::~response() {}

size_t response::read_header(char *buff, size_t size)
{
    size_t ret = 0;
    if (!header_finished)
    {
        if (input_type == INPIPE)
        {
            size_t pos;
            strcpy(buff, header_remaining.c_str());
            size_t remaining_size = header_remaining.length();
            ret = read(fd, buff + remaining_size, size - remaining_size); // non-blocking read
            if (ret == 0)
            {  // eof + header not finished
                header_finished = true;
                status = http::UNAVAILABLE;
            } 
            else if (ret == -1)
                ret = 0; // no data + fd writer still exist
            else
            {
                ret += remaining_size;
                std::string str(buff, ret);
                pos = str.find("\r\n\r\n");
                if (pos != std::string::npos)
                {
                    ret = pos + 4;
                    body << &str[ret];
                    header.write(str.c_str(), ret - 2);
                    input_type = INBODYPIPE;
                    std::string head = header.str();
                    if (head.find("Content-Length", 0, ret) == std::string::npos)
                    {
                        content_len = false;
                        set_header("Transfer-Encoding", "chunked");
                    }
                    pos = head.find("Status: ", 0, ret);
                    strcpy(buff, "HTTP/1.1 202 KO\r\n");
                    if (pos != std::string::npos)
                    {
                        pos += 8;
                        strncpy(buff + 9 , &head[pos] , head.find("\r\n", pos) - pos + 2);
                    }
                    return strlen(buff);
                }
                else
                {
                    header_remaining = str.substr(str.size() - std::min(size_t(3), str.size()));
                    ret -= std::min(size_t(3), str.size());
                    header.write(str.c_str(), ret);
                }
            }
            return 0;
        }
        else
        {
            std::cerr << "reading simple header" << std::endl;
            if (header.tellg() == 0)
                header << "\r\n";
            ret = header.read(buff, size).gcount();
            if (header.bad() || header.eof())
                header_finished = true;
        }
    }
    return ret;
}

size_t response::read_body(char *buff, size_t size)
{
    size_t ret = 0;
    if (!finished)
    {
        if (input_type == STREAM)
        {
            ret = body.read(buff, size).gcount();
            if (body.eof())
                finished = true;
        }
        else if (input_type == INPIPE || input_type == INBODYPIPE)
        {
        	if (content_len == false)
            {
                size -= 20;
                size_t bsize = body.read(buff, size).gcount();
                ret = ::read(fd, buff + bsize, size - bsize);
                if (ret == -1)
                    ret = bsize;
                else if (ret + bsize == 0)
                {
                    strcpy(buff, "0\r\n\r\n");
                    ret = 5;
                    finished = true;
                }
                else
                // no data + fd writer still exist
                // non-blocking read and size must be greater than 22
                {
                    char hex_buff[20];
                    ret += bsize;
                    // std::itoa(ret + bsize, hex_buff, 16);
                    int n = sprintf(hex_buff,"%zx", ret);
                    std::cout << hex_buff << std::endl;
                    strcat(hex_buff + n, "\r\n");
                    memmove(buff + n + 2, buff, ret);
                    memmove(buff, hex_buff, n + 2);
                    std::cout << "reader body|" << std::string(buff, ret+n+2) << "|" << std::endl;
                    memcpy(buff + n + ret + 2, "\r\n", 2);
                    ret += n + 4;
                    // return (result.copy(buff, result.size()));
                }
            }       
        }
        else if (bodyfile->is_open())
        {
            ret = bodyfile->read(buff, size).gcount();
            if (bodyfile->eof() || bodyfile->bad())
                finished = true;
        }
    }
    return ret;
}

bool response::request_valide(IParseable &header)
{
    std::cout << "request_valide" << std::endl;
    if (status != http::OK)
        return generate_response_error(status), false;
    else if (header[0]["uri"].str().size() >= 2048)
        return generate_response_error(http::REQUEST_URI_TOO_LONG), false;
    else if (header[0]["ver"].str() != "HTTP/1.1")
        return generate_response_error(http::VERSION_NOT_SUPPORTED), false;
    else if (allow_method->find(header[0]["method"].str()) == allow_method->end())
        return generate_response_error(http::METHOD_NOT_ALLOWED), false;
    std::cout << "request_valide end" << std::endl;
    return true;
}

void response::generate_response_error(short error, std::string const &msg)
{
    static char const *tmpl = "<div class='main-error-page'>\
    <center> <h1 class='error-title'> %hd   %s </h1>\
    <h2 class='error-subtitle'> sir lah yn... sir fhalk</h2>\
    </center></div>";

    header.clear(); // clear header in case of recursive call
    status = error;
    std::string &error_msg = GS.http_errors.find(error)->second;
    set_header("", "HTTP/1.1 " + std::to_string(error) + "  " + error_msg);

    // get the error page
    std::string error_page;
    if (error_pages != nullptr)
    {
        for (size_t i = 0; i < error_pages->size(); i++)
        {
            std::set<int>::iterator ite = (*error_pages)[i]["codes"].int_set().end();
            if ((*error_pages)[i]["codes"].int_set().find(error) != ite)
            {
                std::string &page = (*error_pages)[i]["page"].str();
                std::cout << joinpath(root, page).c_str() << std::endl;
                if (access(joinpath(root, page).c_str(), F_OK | R_OK) == 0)
                {
                    error_page = page;
                    break;
                }
            }
        }
    }
    if (error_page.empty())
    {
        char buffer[4000];
        size_t len = sprintf(buffer, tmpl, error, (msg == "" ? error_msg : msg).c_str());
        set_header("Content-Length", std::to_string(len));
        this->body << buffer;
    }
    else
        prepare_body(joinpath(root, error_page));
    std::cout << "generate_response_error end" << std::endl;
}

void response::generate_redirect(int status, std::string const &location)
{
    set_header("", "HTTP/1.1 " + std::to_string(status) + " Moved Permanently");
    set_header("Location", location);
    set_header("Content-Length", "0");
    body.clear();
}

void response::generate_autoindex(std::string const &path)
{
    DIR *dir;
    struct dirent *dp;
    char buff[4000];

    body.clear();
    if ((dir = opendir(path.c_str())) == NULL)
    {
        return;
    }
    body << "<html> <head><title>Index of /</title></head>\n";
    body << "<body>\n <hr>\n <pre>\n";
    while ((dp = readdir(dir)) != NULL)
    {
        sprintf(buff, "<a href= '%s' > %s </a>\n", dp->d_name, dp->d_name);
        body << buff;
    }
    body << "</pre>\n <hr>\n </body>\n </html>\n";
    closedir(dir);
    set_header("", "HTTP/1.1 200 OK");
    body.flush();
    set_header("Content-Length", std::to_string(body.str().size()));
}

void response::prepare_body(std::string const &path)
{
    // check if
    std::cout << "Preparing body for " << path << std::endl;
    bodyfile = new std::ifstream(path, std::ifstream::in | std::ios::binary);
    if (bodyfile->is_open())
    {
        set_header("Content-Length", std::to_string(std::ifstream(path, std::ios::ate).tellg()));
        set_header("Content-Type", contentType(path));
        input_type = INFILE;
    }
    else
        generate_response_error(http::NOT_FOUND);
}

char *response::get_header()
{
    header.read(header_buff, header.tellg());
    strcat(&header_buff[header.tellg()], "\r\n");
    return header_buff;
}

short response::get_status()
{
    return status;
}

bool response::is_finished()
{
    return finished;
}

bool response::is_header_finished()
{
    return header_finished;
}

void response::handle_cgi_req(IParseable &rheader, std::string &rbody)
{
    struct stat s;
    std::string path = joinpath(root, cleanpath(rheader[0]["uri"].str()));

    std::cout << "requested filepath: " << path << std::endl;
    if (stat(path.c_str(), &s) == 0)
    {
        if (s.st_mode & S_IFDIR) // Is Directory
        {
            if (path.back() != '/')
                generate_redirect(301, rheader[0]["uri"].str() + "/"); // directory
            else if (join_index(path))
            {
                // fd = call_cgi(rheader, rbody, sconf, loc, path);
                fd = launch_cgi(rheader, *sconf, *loc, rbody, &path[0]);
                if (fd > 0)
                    input_type = INPIPE;
                else
                    generate_response_error(http::INTERNAL_SERVER_ERROR);
            }
            else if (autoindex)
                generate_autoindex(path);
            else
                generate_response_error(http::FORBIDDEN);
        }
        else if (s.st_mode & S_IFREG) // Is File
        {
            fd = launch_cgi(rheader, *sconf, *loc, rbody, &path[0]);
            if (fd > 0)
                input_type = INPIPE;
            else
                generate_response_error(http::INTERNAL_SERVER_ERROR);
        }
        else // something else
            generate_response_error(http::UNAVAILABLE);
    }
    else
        generate_response_error(http::NOT_FOUND); // something else
}

void response::handle_get_req(IParseable &rheader, std::string &rbody)
{
    struct stat s;
    std::string path = joinpath(root, cleanpath(rheader[0]["uri"].str()));

    std::cout << "requested filepath: " << path << std::endl;
    if (stat(path.c_str(), &s) == 0)
    {
        if (s.st_mode & S_IFDIR) // Is Directory
        {
            if (path.back() != '/')
                generate_redirect(301, rheader[0]["uri"].str() + "/"); // directory
            else if (join_index(path))
            {
                set_header("", "HTTP/1.1 200 OK");
                prepare_body(path);
            }
            else if (autoindex)
                generate_autoindex(path);
            else
                generate_response_error(http::FORBIDDEN);
        }
        else if (s.st_mode & S_IFREG) // Is File
        {
            set_header("", "HTTP/1.1 200 OK");
            prepare_body(path);
        }
        else // something else
            generate_response_error(http::UNAVAILABLE);
    }
    else
        generate_response_error(http::NOT_FOUND); // something else
}

void response::handle_get_delete(IParseable &rheader, std::string &rbody)
{
}

std::string response::contentType(std::string path)
{
    std::string ext = extension(path);
    std::map<std::string, std::string>::iterator it;
    it = GS.content_types.find(ext);
    if (it != GS.content_types.end())
    {
        return it->second;
    }
    return "text/plain";
}

bool response::extract_config(IParseable &header)
{
    loc = find_location((*sconf)["location"], header[0]["uri"].str());
    root = (*sconf)["root"].str();
    if (sconf->contains("error_page"))
        error_pages = &(*sconf)["error_page"];
    if (sconf->contains("index"))
        indexs = &(*sconf)["index"].str_set();
    allow_method = &(*sconf)["allow_methods"].str_set();
    autoindex = (*sconf)["autoindex"].str() == "on";
    if (loc)
    {
        if ((*loc)[1].contains("root"))
            root = (*loc)[1]["root"].str();
        IParseable &context = (*loc)[1];
        allow_method = &context["allow_methods"].str_set();
        autoindex = context["autoindex"].str() == "on";
        redire = context.contains("return");
        if (loc->contains("error_page"))
            error_pages = &context["error_page"];
        if (loc->contains("index"))
            indexs = &context["index"].str_set();
    }
    return false;
}

void response::set_header(std::string const &name, std::string const &value)
{
    if (!value.empty())
    {
        header << name;
        if (name != "")
            header << ": ";
        header << value << "\r\n";
    }
}

bool response::join_index(std::string &path)
{
    std::set<std::string>::iterator it = indexs->begin();
    for (; it != indexs->end(); it++)
    {
        std::string tmp = joinpath(path, *it);
        std::cout << path << "   " << *it << std::endl;
        if (access(tmp.c_str(), F_OK | R_OK) == 0)
        {
            path = tmp;
            return true;
        }
    }
    return false;
}
