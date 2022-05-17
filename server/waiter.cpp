#include "waiter.hpp"
#include <algorithm>
#include <fcntl.h>
#include <GlobalStorage.hpp>

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
	if (::poll(&_pfd[0], _pfd.size(), -1) == -1)
		throw std::runtime_error("erro: poll()");
}

void sig_pipe_handler(int n)
{
	GS.sig_pipe = true;
}

void write_save_remainder(int &fd, std::string &buffer, char *to_write, size_t len)
{
	size_t ret;
	signal(SIGPIPE, &sig_pipe_handler);
	ret = write(fd, to_write, len);
	if (ret == -1 && GS.sig_pipe)
		close(fd) , fd = -1;
	else
		buffer.assign(to_write + ret, len - ret);
	signal(SIGPIPE,SIG_DFL);
	GS.sig_pipe = true;
}

void waiter::connections_handler()
{
	int		ret = 0;
	char	buff[1025];

	for (size_t i = GS.nlisteners; i < _pfd.size(); i++)
	{
		request &req = _sockets[i]._request;
		response *resp = req.resp;

		if ((_pfd[i].revents & POLLIN) && (resp == nullptr || resp->is_finished())) // read and handle new request
		{
			ret = read(_sockets[i]._id, buff, 1024);
			if (ret == 0)
			{
				close(_sockets[i]._id), _sockets[i]._id = -1;
				continue;
			}
			if (ret > 0)
			{
				buff[ret] = '\0';
				req.append_data(buff, ret);
				req.handle();
			}
		}
		if (req.get_status() == REQUEST_READY)
		{
			req.gen_response();
			resp = req.resp;
		}
		if (resp && !resp->is_finished() && (_pfd[i].revents & POLLOUT) )
		{
			if (_sockets[i].buffer.size() > 0)
				write_save_remainder(_sockets[i]._id, _sockets[i].buffer, (char*)_sockets[i].buffer.c_str(), _sockets[i].buffer.size());	   
			else if (!req.resp->is_header_finished())
				write_save_remainder(_sockets[i]._id, _sockets[i].buffer, buff,  req.resp->read_header(buff, 1024));
			else
			{
				write_save_remainder(_sockets[i]._id, _sockets[i].buffer, buff,  req.resp->read_body(buff, 1024));
				if ( resp->is_finished() && resp->get_status() > 399 && resp->get_status() < 500)
					close(_sockets[i]._id) , _sockets[i]._id = -1;
			}
		}
	}
}

void waiter::accept()
{
	for (size_t i = 0; i < GS.nlisteners; i++)
	{
		if (_pfd[i].revents & POLLIN)
		{
			sock csock(_sockets[i]._id);
			_sockets[i].accept(csock);
			insert(csock, POLLIN | POLLOUT);
			fcntl(csock._id, F_SETFL, O_NONBLOCK);
		}
	}
}

void waiter::remove()
{
	std::vector<sock>::iterator it = _sockets.begin();
	std::vector<p_fd>::iterator	lit = _pfd.begin();

	while (it != _sockets.end())
	{
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
