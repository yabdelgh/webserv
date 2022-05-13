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
	std::cout << "00000000000000000000" << std::endl;
	std::cout << s._id << std::endl;
	_sockets.push_back(s);
	std::cout << "000000000000000000000" << std::endl;
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
	char buff[1024];
	int j = 0;
	for (size_t i = 0; i < _pfd.size(); i++)
	{
		if (_sockets[i]._status == 1 && (_pfd[i].revents & POLLIN))
		{
			sock csock;
			std::cout << "accept" << std::endl;
			// csock._conf = _sockets[i]._conf;
			csock._request.set_conf(_sockets[i]._conf);
			_sockets[i].accept(csock);
			std::cout <<"|||||||||||||||||" <<_sockets[i]._conf.size() << std::endl;
			insert(csock, POLLIN | POLLOUT);
			std::cout <<"|=|=|=|=|=|=|=|=|" <<_sockets[1]._conf.size() << std::endl;
			fcntl(csock._id, F_SETFL, O_NONBLOCK);
		}
		else if (_sockets[i]._status == 0)
		{
			request &req = _sockets[i]._request;
			std::cout <<"++++++++++++++++++++++++++++++++" <<_sockets[i]._conf.size() << std::endl;
			response *resp = req.resp;
			
			if ((_pfd[i].revents & POLLIN) && (resp == nullptr || resp->is_finished())) // read and handle new request
			{
				j = read(_sockets[i]._id, buff, 1024);
				buff[j] = '\0';
				req.append_data(buff);
				req.handle();
			}
			if (req.get_status() == REQUEST_READY || req.get_status() == BAD_REQUEST)
				req.gen_response();
			if (resp && !resp->is_finished() && (_pfd[i].revents & POLLOUT) )
			{
				int len = 0;
				if ((len = req.resp->read_header(buff, 1024)))
				{
					write(_sockets[i]._id, buff, len);
					if (len < 1024)
						write(_sockets[i]._id, "\r\n", 2); // header body splitter
				}
				else
				{
					len = req.resp->read(buff, 1024);
					write(_sockets[i]._id, buff, len);
					std::cout << resp->get_status() << std::endl;
					if ( resp->is_finished() && resp->get_status() > 399 && resp->get_status() < 500)
					{
						std::cout << "close" << std::endl;
						close(_sockets[i]._id);
						_sockets[i]._id = -1;
					}
				}
			}
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
		if (it->_id == -1)
		{
			_sockets.erase(it);
			_pfd.erase(lit);
		}
		else if (lit->revents & POLLHUP)
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
