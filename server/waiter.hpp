#ifndef __WAITER_HPP__
#define __WAITER_HPP__

#include <poll.h>
#include <iostream>
#include "socket.hpp"
#include <vector>
#include <unistd.h>

class waiter
{
	public:
	typedef struct pollfd p_fd;

	public:
	waiter();
	waiter(const waiter& copy);

	public:
	std::vector<sock> _sockets;
	std::vector<struct pollfd> _pfd;

	public:
	waiter& operator=(const waiter &copy);

	public:
	void	insert(const sock &s, short events);
	void	poll();
	void 	accept();
	void	remove();

	public:
	virtual ~waiter();
};

#endif
