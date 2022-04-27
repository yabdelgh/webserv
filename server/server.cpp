#include <iostream>
#include <cstdlib>
#include <poll.h>
#include <vector>
#include <unistd.h>
#include "socket.hpp"
#include "waiter.hpp"
#include "tools.hpp"

int main(int ac, char **av)
{
	if (ac > 1)
	{
		std::string content;
		get_file_contents(av[1], content);
		std::cout << content << std::endl;
		IParseable &conf = *get_server_config();
		size_t idx = 0;
		std::cout << (conf.parse(content, idx) ? "good" : "bad") << std::endl;

		try
		{
			waiter	serve;
			for (int i = 0; i < conf["server"].size(); i++)
				serve.insert(sock(conf["server"][i]["listen"][0]["host"].get_string().c_str(),
								 		conf["server"][i]["listen"][0]["port"].get_int(), 1), POLLIN);
			while (1)
			{
				serve.poll();
				serve.remove();
				serve.accept();
			}
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	return (0);
}
