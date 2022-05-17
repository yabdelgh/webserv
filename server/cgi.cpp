#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <libgen.h>
#include "IParseable.hpp"

void set_environnement(IParseable &header, IParseable &conf, char *path)
{
	setenv("SERVER_SOFTWARE", "Webserv", 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.2", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REDIRECT_STATUS", "CGI", 1);
	setenv("REQUEST_METHOD", header[0]["METHOD"].str().c_str(), 1);
	setenv("REQUEST_URI", header[0]["URI"].str().c_str(), 1);
	setenv("SERVER_NAME", conf["SERVER_NAME"].str().c_str(), 1);
	setenv("SERVER_PORT", std::to_string(conf["listen"][0]["port"].num()).c_str(), 1);
	setenv("SCRIPT_FILENAME", path, 1);
	if (header[0].contains("QUERY"))
		setenv("QUERY_STRING",header[0]["QUERY"].str().c_str(),1);
	if (header[1].contains("content-type"))
		setenv("CONTENT_TYPE",header[1]["content-type"].str().c_str(),1);
	if (header[1].contains("content-length"))
		setenv("CONTENT_LENGTH",std::to_string(header[1]["content-length"].num()).c_str(),1);
	std::cout << "----------------> cgi setenv 1" << std::endl;
	//if (header[1].contains("SCRIPT_NAME"))
	//	setenv("SCRIPT_NAME",header[1]["SCRIPT_NAME"].str(),1);
	//if (header[1].contains("PATH_INFO"))
	//	setenv("PATH_INFO",header[0]["PAHT_INFO"].str(),1);
	std::cout << "cgi setenv 2" << std::endl;
	if (header[1].contains("HTTP_COOKIE"))
		setenv("HTTP_COOKIE",header[1]["COOKIE"].str().c_str(),1);
	if (header[1].contains("HTTP_ACCEPT"))
		setenv("HTTP_ACCEPT",header[1]["ACCEPT"].str().c_str(),1);
	if (header[1].contains("USER_AGENT"))
		setenv("HTTP_USER_AGENT",header[1]["USER_AGENT"].str().c_str(),1);
	if (header[1].contains("ACCEPT_LANGUAGE"))
		setenv("HTTP_ACCEPT_LANGUAGE",header[1]["ACCEPT_LANGUAGE"].str().c_str(),1);
}

int launch_cgi(IParseable &header,
			   IParseable &sconf,
			   IParseable &loc,
			   std::string &filename,
			   char *path)
{
	try
	{
		pid_t			pid;
		int 			pfd[2];
		int			fd_body;
		char	*arg[3];
		extern char		**environ;

		arg[0] = &loc[1]["cgi_pass"].str()[0];
		arg[1] = path;
		std::cout << arg[0] << " " << arg[1] << std::endl; 
		// arg[1] = (char *)header[0]["URI"].str().c_str();
		arg[2] = NULL;

		if (pipe(pfd) == -1)
			throw std::runtime_error("error: pipe()");
		if ( (pid = fork()) == -1 )
			throw std::runtime_error("error: fork()");
		if (pid == 0)
		{
			set_environnement(header, sconf, path);
			if (header[0]["METHOD"].str() == "POST")
			{
				fd_body = open(filename.c_str(), O_RDONLY);
				if (fd_body == -1)
					throw std::runtime_error("error: launch_cgi() : open()");
				if (fd_body > 2 && (dup2(fd_body, 0) == -1 || close(fd_body)))
					throw std::runtime_error("error: launch_cgi() : dup2() or close()");
			}
			if (dup2(pfd[1], 1) == -1 || close(pfd[1]) == -1 || close(pfd[0]) == -1)
				throw std::runtime_error("error: launch_cgi(): dup2() or close()");
			if (chdir(dirname(path)) == -1 || execve(arg[0], arg, environ) == -1)
				throw std::runtime_error("error: launch_cgi() : chdir() or execve()");
		}
		if (close(pfd[1]) == -1)
			throw std::runtime_error("error: launch_cgi() : close()");	
		return (pfd[0]);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return (-1);
	}
}
