#include <unistd.h>

pid_t create_child(int *pfd, bool post)
{
	int pid;

	if (pipe(pfd))
		throw std::runtime_error("error: pipe()");
	if (post)
		if (pipe(pfd + 2))
			throw std::runtime_error("error: pipe()");
	pid = fork();
	if (pid < 0)
		throw std::runtime_error("error: fork()");
	return (pid);
}

void exec_cgi(int *pfd, bool post)
{
	if (pid == 0)
	{
		if (dup2(pfd[1], 1) == -1)
			throw std::runtime_error("error: dup2()");
		if (post)
			if (dup2(pfd[2], 0))
				throw std::runtime_error("error: dup2()");
		for (int i = 0; i < 4; i++)
			close(pfd[i]);
		if (execve())
			throw std::runtime_error("error: execve()");
		//exit
	}
}

void get_response(int *pfd, bool post)
{
	close(pfd[1]);
	if (post)
	{
		close(pfd[2]);
		write(pfd[3]);
		close(pfd[3]);
	}
	while
	{
		read(pfd[0]);
	}
	close(pfd[0]);
}

void set_environnement()
{
	setenv("SERVER_SOFTWARE", "Webserv", 1);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REQUEST_METHOD", req.get_method(), 1);
	setenv("HTTP_USER_AGENT",req.get("User-Agent"))


}

void launch_cgi()
{
	extern char		**environ;
	int 			pfd[4];
	pid_t			pid;

	pid = create_child(pfd, false);
	exec_cgi();
	get_response();
}
