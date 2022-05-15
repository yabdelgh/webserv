#ifndef GLOBAL_STORAGE_HPP
#define GLOBAL_STORAGE_HPP

#include "IParseable.hpp"
#include <map>
#include <string>
#include <vector>
#include "IParseable.hpp"
#include "Pattern.hpp"
#define GS GlobalStorage::get()

namespace http
{
    enum HttpStatusCode
    {
        OK = 200,
        BAD_REQUEST = 400,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        REQUEST_TIMEOUT = 408,
        GONE = 410,
        LENGTH_REQUIRED = 411,
        PAYLOAD_TOO_LARGE = 413,
        REQUEST_URI_TOO_LONG = 414,
        UNAVAILABLE = 451,
        INTERNAL_SERVER_ERROR = 500,
        VERSION_NOT_SUPPORTED = 505,
        INSUFFICIENT_STORAGE = 507,
    };
}

class GlobalStorage
{
private:
    GlobalStorage();
    static GlobalStorage *instance;
    void set_content_types();
    void set_http_errors();
    void set_patterns();

public:
    IParseable *server_conf;
    std::map<std::string, std::string> content_types;
    std::map<short, std::string> http_errors;
    std::map<std::string, rgx::Pattern> patterns;
    std::map<int , std::vector<IParseable *> > socket_confs;

public:
    ~GlobalStorage();
    static GlobalStorage &get();
};

#endif 