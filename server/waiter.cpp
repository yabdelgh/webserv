#include "waiter.hpp"
#include <algorithm>
#include <fcntl.h>
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
	// std::cout << std::endl << "waiting on poll()" << std::endl;
	if (::poll(&_pfd[0], _pfd.size(), -1) == -1)
		throw std::runtime_error("erro: poll()");
	// std::cout << std::endl << "take the hand()" << std::endl;
	
}

void waiter::accept()
{
	signal(SIGPIPE, SIG_IGN); // tmp
	char buff[1024000];
	int j = 0;
	for (size_t i = 0; i < _pfd.size(); i++)
	{
		if (_sockets[i]._status == 1 && (_pfd[i].revents & POLLIN))
		{
			sock csock;
			std::cout << "accept" << std::endl;
			_sockets[i].accept(csock);
			insert(csock, POLLIN/* | POLLOUT*/);
			fcntl(csock._id, F_SETFL, O_NONBLOCK);
		}
		else if (_sockets[i]._status == 0 && (_pfd[i].revents & POLLIN))
		{
			request &req = _sockets[i]._request;
			response *resp = req.resp;
			if (resp && !resp->is_finished()) // checking for old uncompleted response
			{
				size_t len = resp->read(buff, 1024);
				write(_sockets[i]._id, buff, len);
			}
			if (resp == nullptr || resp->is_finished()) // read and handle new request
			{
				j = read(_sockets[i]._id, buff, 1024);
				buff[j] = '\0';
				req.append_data(buff);
				req.handle();
			}
			if (req.get_status() == REQUEST_READY || req.get_status() == BAD_REQUEST) // generate new response and write header 
			{
				req.gen_response();
				size_t len = req.resp->read_header(buff, 102400); // may need to read all
				write(_sockets[i]._id, buff, len);
				write(_sockets[i]._id, "\r\n", 2); // header body splitter
				write(1, buff, len);
				write(1, "\r\n", 2);
				// while testing read body here
				len = req.resp->read(buff, 1024000);
				write(_sockets[i]._id, buff, len);
				write(1, buff, len);
			}
			// std::cout << "read" << std::endl;
			// j = read(_sockets[i]._id, buff, 1024);
			// buff[j] = '\0';
			// _sockets[i]._request.append_data(buff);
			// _sockets[i]._request.handle();
			// if (_sockets[i]._request.get_status() == REQUEST_READY)
			// {
			// 	response &resp = _sockets[i]._request.get_response();
			// 	std::cout << "got response" << std::endl;
			// 	size_t len = resp.read(buff, 1024);
			// 	if (len < 1024)
			// 		_sockets[i]._request.set_status(INCOMPLETE_HEADER);
			// 	std::cout << "response: " << buff << std::endl;
			// 	write(_sockets[i]._id, buff, len);
			// }
			// if (_sockets[i]._request.get_status() == BAD_REQUEST)
			// {
			// 	response &resp = _sockets[i]._request.get_response();
			// 	size_t len = resp.read(buff, 1024);
			// 	if (len < 1024)
			// 		_sockets[i]._request.set_status(INCOMPLETE_HEADER);
			// 	std::cout << "response: " << buff << std::endl;
			// 	write(_sockets[i]._id, buff, len);
			// 	std::cout << "got response" << std::endl;
			// }
			// std::list<response> resps = _sockets[i]._request.pop_responses();
			// std::cout << "got response" << std::endl;
			// std::list<response>::iterator it = resps.begin();
			// for (; it != resps.end() ; it++)
			// {
			// 	size_t len = it->read(buff, 5666);
			// 	write(_sockets[i]._id, buff, len);
			// }
			// write(1,buff,j);
		}
	/*	if (_sockets[i]._status == 0 && (_pfd[i].revents & POLLOUT))
		{
			int ret = 7;
			while (ret == 7)
			{
				ret = write(_sockets[i]._id, "yassine", 7);
				std::cout << "ret:    " << ret << std::endl;
			}
			perror("error");
			std::cout << "ret:    " << ret << std::endl;
		}*/
		
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
