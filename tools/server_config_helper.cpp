#include "server_config_helper.hpp"
#include "IParseable.hpp"
#include "complex_types/Directive.hpp"
#include "simple_types/String.hpp"
#include "simple_types/Int.hpp"
#include "defined_patterns.hpp"
#include "tools.hpp"
#include "complex_types/Frequent.hpp"

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

IParseable *find_location(IParseable &locations, std::string const &path, int depth)
{
    std::string ext = "*." + extension(path);
    if (depth == 0 && ext != "*.")
    {
        IParseable *location = find_location(locations, ext, 1);
        if (location)
        {
            std::cout << "goast location: " << std::endl;
            return location;
        }
    }
    for (size_t i = 0; i < locations.size(); i++)
    {
        std::string const &uri = locations[i]["uri"].str();
        if (uri.size() <= path.size())
        {
            if (::strncasecmp(uri.c_str(), path.c_str(), uri.size()) == 0)
            {
                std::cout << "location: " << i << std::endl;
                return &locations[i];
            }
        }
    }
    return nullptr;
}

size_t get_client_body_limit(IParseable &conf, IParseable *location)
{
    size_t client_body_limit;
    char unit;
    client_body_limit = conf["client_body_buffer_size"]["limit"].num();
    unit = conf["client_body_buffer_size"]["unit"].str()[0];
    if (location)
    {
        client_body_limit = (*location)[1]["client_body_buffer_size"]["limit"].num();
        unit = (*location)[1]["client_body_buffer_size"]["unit"].str()[0];

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

bool compareLocations(IParseable *l1, IParseable *l2)
{
    return ((*l1)["uri"].str().size() > (*l2)["uri"].str().size());
}

void sort_locations(IParseable &serv_conf)
{
    for (size_t i = 0; i < serv_conf.size(); i++)
    {
        if (serv_conf[i].contains("location"))
        {
            std::vector<IParseable *> &locations = static_cast<Frequent &>(serv_conf[i]["location"]).get_parseables();
            if (locations.size() > 0)
                sort(locations.begin() ,locations.end() ,compareLocations);
        }
    }
}

