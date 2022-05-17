#include <iostream>
#include <cstdlib>
#include <poll.h>
#include <vector>
#include <unistd.h>
#include "socket.hpp"
#include "waiter.hpp"
#include "tools.hpp"
#include "GlobalStorage.hpp"
#include <map>
#include <utility>
#include "server_config_helper.hpp"


std::map<std::pair<std::string, short>, short > get_listeners(IParseable &conf)
{
	std::map<std::pair<std::string, short> , short> mm;
	for (int i = 0; i < conf["server"].size(); i++)
		for(int j = 0; j < conf["server"][i]["listen"].size(); j++)
			mm.insert(std::make_pair( std::make_pair(conf["server"][i]["listen"][j]["host"].get_string()
					,conf["server"][i]["listen"][j]["port"].get_int()), POLLIN));
	return (mm);
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
			sort_locations(conf["server"]);
			GS.server_conf = &conf["server"];
			waiter	serve;
			serve.insert(get_listeners(conf), conf);

			while (1)
			{
				serve.poll();
				serve.remove();
				serve.accept();
				serve.remove();
			}
		}
		catch (std::exception &e)
		{
			std::cout << "error: " << e.what() << std::endl;
			perror("tata");
		}
	}
	return (0);
}
