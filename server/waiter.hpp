#ifndef __WAITER_HPP__
#define __WAITER_HPP__

#include <poll.h>
#include <iostream>
#include "socket.hpp"
#include <vector>
#include <unistd.h>
#include "request.hpp"
#include <map>

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
	std::map<int, request> _req;

	public:
	waiter& operator=(const waiter &copy);
	
	template <class CONTAINER>
	void insert(CONTAINER c, IParseable &conf)
	{
		typename CONTAINER::iterator it = c.begin();
		typename CONTAINER::iterator ite = c.end();
    	while (it != ite)
    	{
		sock tmp(it->first.first.c_str(), it->first.second, 1);
	for (int i = 0; i < conf["server"].size(); i++)
	{
		for (int j = 0; j < conf["server"][i]["listen"].size(); j++)
		{
			if (it->first.first == conf["server"][i]["listen"][j]["host"].get_string()
			&& it->first.second == conf["server"][i]["listen"][j]["port"].get_int())
				tmp.conf.push_back(&conf);
		}
	}
       	 insert(tmp, it->second);
       	 it++;
    	}
	}

	public:
	void	insert(const sock &s, short events);
	void	poll();
	void 	accept();
	void	remove();

	public:
	virtual ~waiter();
};

#endif
