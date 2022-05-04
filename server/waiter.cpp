#include "waiter.hpp"

waiter::waiter() : _sockets(), _pfd() {}

waiter::waiter(const waiter & copy) : _sockets(copy._sockets), _pfd(copy._pfd) {}

waiter& waiter::operator=(const waiter &copy)
{
	_sockets = copy._sockets;
	_pfd = copy._pfd;
	return (*this);
}

void waiter::insert(const sock &s, short events)
{
	p_fd tmp;

	tmp.fd = s._id;
	tmp.events = events;
	_pfd.push_back(tmp);
	_sockets.push_back(s);
}

void waiter::poll()
{
	std::cout << std::endl << "waiting on poll()" << std::endl;
	if (::poll(&_pfd[0], _pfd.size(), -1) == -1)
		throw std::runtime_error("erro: poll()");
}

void waiter::accept()
{
	sock csock;
	char buff[1024];
	int j = 0;
	for (size_t i = 0; i < _pfd.size(); i++)
	{
		if (_sockets[i]._status == 1 && (_pfd[i].revents & POLLIN))
		{
			std::cout << "accept" << std::endl;
			_sockets[i].accept(csock);
			insert(csock, POLLIN);
		}
		else if (_sockets[i]._status == 0 && (_pfd[i].revents & POLLIN))
		{
			std::cout << "read" << std::endl;
			j = read(_sockets[i]._id, buff, 10);
			buff[j] = '\0';
			_sockets[i]._request.append_data(buff);
			std::list<response> resps = _sockets[i]._request.pop_responses();
			std::cout << "got response" << std::endl;
			std::list<response>::iterator it = resps.begin();
			for (; it != resps.end() ; it++)
			{
				size_t len = it->read(buff, 5666);
				write(_sockets[i]._id, buff, len);
			}
			
			write(1,buff,j);
		}
	}
}

void waiter::remove()
{
	std::vector<sock>::iterator it = _sockets.begin();
	std::vector<p_fd>::iterator	lit = _pfd.begin();

	while (it != _sockets.end())
	{
	//	if (!(lit->revents & POLLIN) && (lit->revents & POLLHUP))
		if (lit->revents & POLLHUP)
		{
			
			std::cout << "close" << std::endl;
			close(it->_id);
			_sockets.erase(it);
			_pfd.erase(lit);
		}
		else
		{
			it++;
			lit++;
		}
	}
}

waiter::~waiter() {}
