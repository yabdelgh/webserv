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
	//	std::cout << "servers: " <<  server_config["server"][1]["listen"].size() << std::endl;
	//	std::cout << "port:" << server_config["server"][0]["listen"][0]["host"].get_string() << std::endl;

		try
		{
			waiter	serve;
			sock socket;
		//	sock	lsock("127.0.0.1", 3000, 1);
		//	sock	psock("0.0.0.0", 3000, 1);
			std::cout << conf["server"].size() << std::endl;
			for (int i = 0; i < conf["server"].size(); i++)
		//	for (int i = 0; i < 2; i++)
			{
				socket = sock(conf["sever"][i]["listen"][0]["host"].get_string().c_str(),
								 		conf["server"][i]["listen"][0]["port"].get_int(), 1);
			std::cout << "--------------------------------------" << std::endl;
				std::cout << socket._id << std::endl;
				socket.default_config();
				serve.insert(socket, POLLIN);
		//		listenrs.back().default_config();
			}

//			lsock.default_config();
//			psock.default_config();
	//		serve.insert(lsock, POLLIN);
	//		serve.insert(psock, POLLIN);	
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
