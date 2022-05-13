#include "server_config_helper.hpp"
#include "IParseable.hpp"
#include "complex_types/Directive.hpp"
#include "simple_types/String.hpp"
#include "simple_types/Int.hpp"

#include "defined_patterns.hpp"

bool compare_ip(std::string const &ip1, std::string const &ip2)
{
    size_t val1, val2, idx1 = 0, idx2 = 0;
    try
    {
        return std::stoi(&ip1[idx1], &idx1) == std::stoi(&ip1[idx2], &idx2) &&
               std::stoi(&ip1[idx1 + 1], &idx1) == std::stoi(&ip1[idx2 + 1], &idx2) &&
               std::stoi(&ip1[idx1 + 1], &idx1) == std::stoi(&ip1[idx2 + 1], &idx2) &&
               std::stoi(&ip1[idx1 + 1], &idx1) == std::stoi(&ip1[idx2 + 1], &idx2);
    } catch (const std::exception)
    {
        return false;
    }
}


IParseable *find_server_conf(IParseable &conf, std::string const &host)
{
    std::cout << "++++++++++++++++++++++++++++++++" << std::endl;
    std::map<std::string, rgx::Pattern> p = get_patterns();
    Directive listen;
    listen.push_parseable(String(p["spaces"]));
    listen.push_parseable("host",String(p["ip"]));
    listen.push_parseable(String(p["colon"]));
    listen.push_parseable("port", Int(p["number"]));
    size_t idx = 0;
    size_t i;
    std::cout << "++++++++++++++++++++++++++++++++" << std::endl;
    if (listen.parse(host, idx) && idx == host.size()) // TODO should accept const string
    {
        for (i = conf.size() - 1; i > 0; i--)
        {
            std::string const &ip1 = conf[i]["listen"]["host"].get_string();
            std::string const &ip2 = listen["host"].get_string();
            int port1 = listen["port"].get_int();
            int port2 = conf[i]["listen"]["port"].get_int();
            if (compare_ip(ip1, ip2) && port1 == port2)
                break;
        }
    }
    else 
        for (i = conf.size() - 1; i > 0; i--)
            if (conf[i]["server_name"].get_string() == host)
                break;
    return &conf[i];
}

IParseable *find_location(IParseable &locations, std::string const &path)
{
    std::cout << "-------------------------------------" << std::endl;
    for (size_t i = 0; i < locations.size(); i++)
    {
        std::string const &uri = locations[i]["uri"].str();
        if (::strncasecmp(uri.c_str(), path.c_str(), uri.size()) == 0)
        {
            return &locations[i];
        }
    }
    std::cout << "-------------------------------------" << std::endl;
    return nullptr;
}