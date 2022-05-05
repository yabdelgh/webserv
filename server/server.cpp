#include <iostream>
#include <cstdlib>
#include <poll.h>
#include <vector>
#include <unistd.h>
#include "socket.hpp"
#include "waiter.hpp"
#include "tools.hpp"
#include <map>
#include <utility>


std::map<std::pair<std::string, short>, short > matsemihach(IParseable &conf)
{
	std::map<std::pair<std::string, short> , short> mm;
	for (int i = 0; i < conf["server"].size(); i++)
		for(int j = 0; j < conf["server"][i]["listen"].size(); j++)
			mm.insert(std::make_pair(
						std::make_pair(conf["server"][i]["listen"][j]["host"].get_string(),conf["server"][i]["listen"][j]["port"].get_int()), POLLIN));
	return mm;
}

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
			serve.insert(matsemihach(conf));

			while (1)
			{
				serve.poll();
				serve.remove();
				serve.accept();
			}
		}
		catch (std::exception &e)
		{
			std::cout << "error: " << e.what() << std::endl;
		}
	}
	return (0);
}
