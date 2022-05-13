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
#include <set>

response::response() {}
response::response(IParseable &header, IParseable &body, short status) : pos(0)
{
    input_type = STREAM;
    sconf = nullptr;
    loc = nullptr;
    error_pages = nullptr;
    allow_method = nullptr;
    indexs = nullptr;
    autoindex = false;
    redire = false;
    finished = false;
    this->status = status;
    content_len = true;
    fd = -1;

    std::cout << "start response" << std::endl;
    if (extract_config(header))
        this->status = (status == http::OK ? http::BAD_REQUEST : status);
    std::cout << "config extraction done" << std::endl;
    if (!request_valide(header, body))
        return;
    std::cout << "method: " << header[0]["method"].str() << std::endl;
    if (redire)
    {
        IParseable &redire = (*loc)["return"];
        generate_redirect(redire["status"].num(), redire["uri"].str());
    }
    else if (header[0]["method"].str() == "GET")
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
        if (0)
        {
	/*	size_t pos;
         	ret = read(fd, buff, size); // non-blocking read
		if (ret == 0)
			error_header(); // eof + header not finished
		else if (ret == -1)
			return (0) // no data + fd writer still exist
		else
		{
			std::string str(buff, ret);
			if (str.find("CONTENT_LENGHT") != std::string::npos)
				content_len = true;
			pos = str.find("\r\n\r\n");
			if (pos != std::string::npos && pos + 4 != str.size)
			{
				stream << str.substr(pos + 4);
				// header finished
				return (pos + 4);
			}
		}*/
        }
        else
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
	else if (0)
	{
	/*	if (content_lenght == false)
		{
			std::stringstream sstream;
			sstream << std::hex << size;
			std::string result = sstream.str();
			ret = ::read(fd, buff, size - 6(\r\n * 3)  - result.size());
			if (ret == 0)
				//end_body() +  return val;
			else if (ret == -1)
				return (0) // no data + fd writer still exist
			// non-blocking read and size must be greater than 22
			sstream << std::hex << ret;
			result	 = sstream.str() + "\r\n";
			result.append(buff, ret);
			resutl	+= "\r\n\r\n";
			return (result.copy(buff, result.size()));
		}
		ret = ::read(fd, buff, size);
		if (ret == 0)
			//end_body() +  return val;
		else if (ret == -1)
			return (0) // no data + fd writer still exist
		return (ret); */
		
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

bool response::request_valide(IParseable &header, IParseable &body)
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

bool response::is_finished()
{
    return finished;
}

void response::handle_get_req(IParseable &header, IParseable &body)
{
    struct stat s;
    std::string path = joinpath(root, cleanpath(header[0]["uri"].str()));

    if (stat(path.c_str(), &s) == 0)
    {
        if (s.st_mode & S_IFDIR) // Is Directory
        {
            if (path.back() != '/')
                generate_redirect(301, header[0]["uri"].str() + "/"); // directory
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

void response::handle_post_req(IParseable &header, IParseable &body)
{
}

void response::handle_get_delete(IParseable &header, IParseable &body)
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
    if (!header[1].contains("host"))
        return true;
    std::string &host = header[1]["host"]["value"].str();
    sconf = find_server_conf(*GS.server_conf, host);
    if (sconf == nullptr)
        return true;
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
        loc = &loc[1];
        allow_method = &(*loc)["allow_methods"].str_set();
        autoindex = (*loc)["autoindex"].str() == "on";
        redire = (*loc).contains("return");
        if (loc->contains("error_page"))
            error_pages = &(*loc)["error_page"];
        if (loc->contains("index"))
            indexs = &(*loc)["index"].str_set();
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
