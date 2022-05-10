#ifndef GLOBAL_STORAGE_HPP
#define GLOBAL_STORAGE_HPP

#include "IParseable.hpp"
#include <map>
#include <string>
#define GS GlobalStorage::get()

class GlobalStorage
{
private:
    GlobalStorage();
    static GlobalStorage *instance;
    void set_content_types();
    void set_http_errors();

public:
    IParseable *server_conf;
    std::map<std::string, std::string> content_types;
    std::map<short, std::string> http_errors;

public:
    ~GlobalStorage();
    static GlobalStorage &get();
};

#endif 