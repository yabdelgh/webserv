#include "socket.hpp"
#include <iostream>
#include <unistd.h>

sock::sock(const char *addr, short port, bool status)
{
	std::cout << "------------------------------------------" << std::endl;
	if (addr != nullptr)
	std::cout << "addr: " << addr << std::endl;
	std::cout << "port: " << port << std::endl;
	_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_id == -1)
		throw std::runtime_error("error: socketttttt()");
	if (addr != nullptr)
		_sin.sin_addr.s_addr = inet_addr(addr);
	if (_sin.sin_addr.s_addr == static_cast<in_addr_t>(-1))
		throw std::runtime_error("error: inet_addr()"); 
	// inet_addr is problematic because -1 is a valid address (255.255.255.255)
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_size = sizeof(_sin);
	_status = status;
	if (status)
		this->default_config();
}

sock::sock(const sock &copy)
{
	_id = copy._id;
	_status = copy._status;
	_size = copy._size;
	_sin = copy._sin;
	_conf = copy._conf;
	_request = copy._request;
	std::cout << _id << " socket copy constructor called" << std::endl;
}

sock& sock::operator=(const sock &copy)
{
	_id = copy._id;
	_status = copy._status;
	_size = copy._size;
	_sin = copy._sin;
	_conf = copy._conf;
	_request = copy._request;
	std::cout << _id << " socket assi constructor called"<< std::endl;
		return (*this);	
}

void sock::bind()
{
	int status;

std::cout << "bind" << std::endl;
	status = ::bind(_id, reinterpret_cast<struct sockaddr*> (&_sin), _size);
	if (status == -1)
		throw std::runtime_error("error: bind()");
}

void sock::listen()
{
	if (::listen(_id, -1) == -1)
		throw std::runtime_error("error: listen()");
}

void sock::accept(sock &c)
{
	c._id = ::accept(_id, reinterpret_cast<struct sockaddr*>(&c._sin), &c._size);	
	if (c._id == -1)
		throw std::runtime_error("error: accept()");
}

void sock::reuseaddr()
{
	int enable = 1;
	if (setsockopt(_id, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw std::runtime_error("error: setsockopt()");
}

void sock::default_config()
{
	reuseaddr();
	bind();
	listen();
}

sock::~sock()
{
		std::cout << "socket destructor called" << std::endl;
}
