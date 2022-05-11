#include "./response.hpp"
#include "./request.hpp"
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "tools.hpp"
#include "server_config_helper.hpp"
#include "GlobalStorage.hpp"
#include <fstream>
#include <iostream>

response::response() {}
response::response(IParseable &header, IParseable &body, bool bad_req) : pos(0)
{
    input_type = STREAM; 
    sconf = nullptr;
    loc = nullptr;
    error_pages = nullptr;
    allow_method = nullptr;
    indexs = nullptr;
    autoindex = false;
    redire  = false;
    finished = false;
    
    std::cout << "start response" << std::endl;
    if (!bad_req)
        bad_req = extract_config(header);
    std::cout << "config extraction done" << std::endl;
    if (!request_valide(header, body, bad_req))
        return;
    std::cout << "method: " << header[0]["method"].str() << std::endl;
    if (header[0]["method"].str() == "GET")
        handle_get_req(header, body);
    else if (header[0]["method"].str() == "POST")
        handle_post_req(header, body);
    else if (header[0]["method"].str() == "DELETE")
        handle_get_delete(header, body);
}

response::~response() {}

size_t response::read_header(char *buff, size_t size)
{
    size_t ret = 0;
    if (!finished)
    {
        ret = header.read(buff, size).gcount();
        // if (body.tellg() == 0)
        //     finished = true;    
    }
    return ret;
}

size_t response::read(char *buff, size_t size)
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
        else if (bodyfile->is_open())
        {
            // memset(buff, 0, size);
            ret = bodyfile->read(buff, size).gcount();
            // buff[ret] = 0;
            // std::cout << "buffile:" << ret <<" "<< buff << "|" << std::endl;
            if (bodyfile->eof())
                finished = true;
        }
        
    }
    return ret;
}

bool response::request_valide(IParseable &header, IParseable &body, bool bad_req)
{
    std::cout << "request_valide" << std::endl;
    if (bad_req)
        return  std::cout << "1\n", generate_response_error(400), false;
    else if (header[0]["uri"].str().size() >= 2048)
        return  std::cout << "2\n", generate_response_error(414), false;
    else if (header[0]["ver"].str() != "HTTP/1.1")
        return  std::cout << "3\n", generate_response_error(505), false;
    else if (allow_method->find(header[0]["method"].str()) == allow_method->end())
        return  std::cout << "4\n", generate_response_error(405), false;
    std::cout << "request_valide end" << std::endl;
    return true;
}

void response::generate_response_error(short error, std::string const &msg)
{
    static char const *tmpl = "<div class='main-error-page'>\
    <center> <h1 class='error-title'> %hd   %s </h1>\
    <h2 class='error-subtitle'> sir lah yn... sir fhalk</h2>\
    </center></div>";

    std::cout << "generate_response_error " << error << std::endl;;
    std::string &error_msg = GS.http_errors.find(error)->second;
    char buffer[4000];
    size_t len = sprintf(buffer, tmpl, error, (msg == "" ? error_msg : msg).c_str());
    if(GS.http_errors.find(error) == GS.http_errors.end())
        std::cout << "error not found" << std::endl;
    std::cout << "|" << buffer << "|" << std::endl;
    set_header("", "HTTP/1.1 " + std::to_string(error) + "  " + error_msg);
    set_header("Content-Length", std::to_string(len));

    this->body << buffer;
    std::cout << "generate_response_error end" << std::endl;
}

void response::generate_redirect(int status, std::string const& location)
{
    set_header("", "HTTP/1.1 " + std::to_string(status) + " Moved Permanently");
    set_header("Location", location);
    set_header("Content-Length", "0");
    body.clear();
}

void response::generate_autoindex(std::string const&path)
{
	DIR *dir;
    struct dirent *dp;
    char buff[4000];

    body.clear();
    if ((dir = opendir (path.c_str())) == NULL)
	{
		return;
	}
	body << "<html> <head><title>Index of /</title></head>\n";
    body << "<body>\n <hr>\n <pre>\n";
    while ((dp = readdir (dir)) != NULL)
	{   sprintf(buff, "<a href= '%s' > %s </a>\n",  dp->d_name, dp->d_name);
		body << buff;
	}
	body << "</pre>\n <hr>\n </body>\n </html>\n";
	closedir(dir);
    set_header("", "HTTP/1.1 200 OK");
    body.flush();
    set_header("Content-Length", std::to_string(body.str().size()));
}

void response::prepare_body(std::string const&path)
{
    // check if

    std::cout << "Preparing body for " << path << std::endl;
    // bodyfile = new std::ifstream(path, std::ios::binary | std::ios::ate);
    bodyfile = new std::ifstream(path, std::ifstream::in | std::ios::binary);
    if (bodyfile->is_open())
    {
        set_header("", "HTTP/1.1 200 OK");
        set_header("Content-Length", std::to_string(std::ifstream(path, std::ios::ate).tellg()));
        set_header("Content-Type",  contentType(path));
        input_type = INFILE;
    }
    else 
        generate_response_error(404);
    
}


char *response::get_header()
{
    header.read(header_buff, header.tellg());
    strcat(&header_buff[header.tellg()] ,"\r\n");
    return header_buff;
}

bool response::is_finished()
{
    return finished;
}

void response::handle_get_req(IParseable &header, IParseable &body)
{
    std::cout << "welcome to get response " << cleanpath(root) << std::endl;
    struct stat s;
    if (redire)
    {
        IParseable &redire = (*loc)["return"];
        generate_redirect(redire["status"].num(), redire["uri"].str());
    }

    std::string path = cleanpath(joinpath(root, header[0]["uri"].str()));
    if (stat(path.c_str(), &s) == 0)
    {
        if (s.st_mode & S_IFDIR)
        {
            if (path.back() != '/')
                generate_redirect(301, header[0]["uri"].str() + "/"); // directory
            else if (join_index(path))
                prepare_body(path);
            else 
                generate_autoindex(path);
                
        }
        else if(s.st_mode & S_IFREG)
            prepare_body(path); // file
        else
            generate_response_error(451); // something else
    }
    else
        generate_response_error(404);
}

void response::handle_post_req(IParseable &header, IParseable &body)
{

}

void response::handle_get_delete(IParseable &header, IParseable &body)
{

}

std::string response::contentType (std::string path)
{
	std::string	ext = extension(path);
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
    if (!header[1].contains("host"))
        return true;
    std::cout << "********************************" << std::endl;
    std::string &host = header[1]["host"]["value"].str();
    std::cout << "********************************" << std::endl;
    sconf = find_server_conf(*GS.server_conf, host);
    std::cout << "********************************" << std::endl;
    if (sconf == nullptr)
        return true;
    loc = find_location((*sconf)["location"], header[0]["uri"].str());
    root = (*sconf)["root"].str();
    std::cout << "********************************" << std::endl;
    if (sconf->contains("error_page"))
        error_pages = &(*sconf)["error_page"];
    if (sconf->contains("index"))
        indexs = &(*sconf)["index"].str_set();
    allow_method = &(*sconf)["allow_methods"].str_set();
    std::cout << "********************************" << std::endl;
    if (loc)
    {
        if ((*loc)[1].contains("root"))
            root = (*loc)[1]["root"].str();
        loc = &loc[1];
        allow_method = &(*loc)["allow_methods"].str_set();
        autoindex = (*loc)["autoindex"].str() == "on" ? true : false;
        redire = (*loc).contains("return");
        if (loc->contains("error_page"))
            error_pages = &(*loc)["error_page"];
        if (loc->contains("index"))
            indexs = &(*loc)["index"].str_set();
    }
    return false;
}

void response::set_header(std::string const& name, std::string const& value)
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
        if (access(tmp.c_str(), F_OK | R_OK) == 0)
        {
            path = tmp;
            return true;
        }
    }
    return false;
}
