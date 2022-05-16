#ifndef SERVER_CONFIG_HELPER_HPP
#define  SERVER_CONFIG_HELPER_HPP

#include "IParseable.hpp"

size_t get_client_body_limit(IParseable &conf, IParseable *location);
IParseable *find_server_conf(std::vector<IParseable *> &confs, std::string const &host);
IParseable *find_server_conf(IParseable &conf, std::string const &host);
IParseable *find_location(IParseable &locations, std::string const &path, int depth = 0);
void sort_locations(IParseable &serv_conf);

#endif //