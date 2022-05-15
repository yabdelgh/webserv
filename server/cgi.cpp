#include <unistd.h>

void set_environnement(IParseable &header, IParseable &conf)
{
	setenv("SERVER_SOFTWARE", "Webserv", 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REQUEST_METHOD", header[0]["METHOD"].str(), 1);
	setenv("REQUEST_URI", header[0]["URI"].str(), 1);
	setenv("SERVER_NAME", conf[0]["SERVER_NAME"].str(), 1);
	setenv("SERVER_PORT", conf[0]["SERVER_PORT"].str(), 1);
	if (header[0]["QUERY_STRING"])
		setenv("QUERY_STRING",header[1]["query_string"].str(),1);
	if (header[1].contains("content_type"))
		setenv("CONTENT_TYPE",header[1]["content_type"].str(),1);
	if (header[1].contains("content_length"))
		setenv("CONTENT_LENGTH",header[1]["content_length"].str(),1);
	//if (header[1].contains("SCRIPT_NAME"))
	//	setenv("SCRIPT_NAME",header[1]["SCRIPT_NAME"].str(),1);
	//if (header[1].contains("PATH_INFO"))
	//	setenv("PATH_INFO",header[0]["PAHT_INFO"].str(),1);
	if (header[1].contains("HTTP_COOKIE"))
		setenv("HTTP_COOKIE",header[1]["COOKIE"].str(),1);
	if (header[1].contains("HTTP_ACCEPT"))
		setenv("HTTP_ACCEPT",header[1]["ACCEPT"].str(),1);
	if (header[1].contains("USER_AGENT"))
		setenv("HTTP_USER_AGENT",header[1]["USER_AGENT"].str(),1);
	if (header[1].contains("ACCEPT_LANGUAGE"))
		setenv("HTTP_ACCEPT_LANGUAGE",header[1]["ACCEPT_LANGUAGE"].str(),1);
}

int launch_cgi(IParseable &header, IParseable &loc, std::string &filename, const char *path)
{
	try
	{
		pid_t			pid;
		int 			pfd[2];
		int			fd_body;
		const char		*arg[3];
		extern char		**environ;

		arg[0] = path;
		arg[1] = header[0]["URI"].str().c_str();
		arg[2] = NULL;

		if (pipe(pfd) == -1)
			throw std::runtime_error("error: pipe()");
		if ( (pid = fork()) == -1 )
			throw std::runtime_error("error: fork()");
		if (pid == 0)
		{
			set_environnement();
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
			if (chdir(path) == =1 || execve(arg[0], arg, environ) == -1)
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
