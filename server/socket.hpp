#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <sys/errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdexcept>
#include <cstdio>
#include <arpa/inet.h>

class sock // AF_INET AND SOCK_STREAM
{
	
	public:
	sock(const char *addr = nullptr, const short port = 0, const bool status = 0);
	sock(const sock &copy);
	
	public:
	int					_id;
	bool				_status;	// 1 listener 0 connexion
	unsigned int		_size;		// address size
	struct	sockaddr_in	_sin;

	public:
	sock& operator=(const sock &copy);

	public:
	void bind();
	void listen();
	void accept(sock &c);
	void reuseaddr();
	void default_config();

	public:
	virtual ~sock();
};

#endif
