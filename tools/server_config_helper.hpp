#ifndef SERVER_CONFIG_HELPER_HPP
#define  SERVER_CONFIG_HELPER_HPP

#include "IParseable.hpp"

IParseable *find_server_conf(IParseable &conf, std::string const &host);
IParseable *find_location(IParseable &locations, std::string const &path);

#endif //