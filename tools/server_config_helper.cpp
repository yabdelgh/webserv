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

IParseable *find_server_conf(std::vector<IParseable *> &confs, std::string const &host)
{
    size_t i;

    std::cout << "confs size " << confs.size() << std::endl;
    for (i = confs.size() - 1; i > 0; i--)
    {
        if ((*confs[i])["server_name"].str() == host)
            break;
    }
    
    return confs[i];
}


IParseable *find_server_conf(IParseable &conf, std::string const &host)
{

    size_t idx = 0;
    size_t i; 
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

size_t get_client_body_limit(IParseable &conf, IParseable *location)
{
    size_t client_body_limit;
    char unit;
    client_body_limit = conf["client_body_buffer_size"]["limit"].num();
    unit = conf["client_body_buffer_size"]["unit"].str()[1];
    if (location)
    {
        client_body_limit = (*location)["client_body_buffer_size"]["limit"].num();
        unit = (*location)["client_body_buffer_size"]["unit"].str()[1];
    }
    switch (tolower(unit))
    {
        case 'g':
            client_body_limit *= 1024;
        case 'm':
            client_body_limit *= 1024;
        case 'k':
            client_body_limit *= 1024;
    }
    return client_body_limit;
}