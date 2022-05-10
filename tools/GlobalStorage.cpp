#include "GlobalStorage.hpp"
#include <utility>

GlobalStorage* GlobalStorage::instance;

GlobalStorage::GlobalStorage() 
{
    set_content_types();
    set_http_errors();
}
GlobalStorage::~GlobalStorage() {}

void GlobalStorage::set_content_types()
{
    content_types.insert(std::make_pair("html",  "text/html"));
    content_types.insert(std::make_pair("htm",   "text/html"));
    content_types.insert(std::make_pair("mp4",   "video/mp4"));
    content_types.insert(std::make_pair("aac",   "audio/aac"));
    content_types.insert(std::make_pair("css",   "text/css"));
    content_types.insert(std::make_pair("gif",   "image/gif"));
    content_types.insert(std::make_pair("ico",   "image/vnd.microsoft.icon"));
    content_types.insert(std::make_pair("jpeg",  "image/jpeg"));
    content_types.insert(std::make_pair("jpg",   "image/jpeg"));
    content_types.insert(std::make_pair("js",    "text/javascript"));
    content_types.insert(std::make_pair("json",  "application/json"));
    content_types.insert(std::make_pair("mp3",   "audio/mpeg"));
    content_types.insert(std::make_pair("mpeg",  "audio/mpeg"));
    content_types.insert(std::make_pair("mp4",   "video/mp4"));
    content_types.insert(std::make_pair("png",   "image/png"));
    content_types.insert(std::make_pair("pdf",   "application/pdf"));
    content_types.insert(std::make_pair("php",   "application/x-httpd-php"));
    content_types.insert(std::make_pair("sh",    "application/x-sh"));
    content_types.insert(std::make_pair("plain", "text/plain"));
    content_types.insert(std::make_pair("svg", "image/svg+xml"));
    content_types.insert(std::make_pair("xml ", "application/xml"));
}

void GlobalStorage::set_http_errors()
{
    // 400 error codes
    http_errors.insert(std::make_pair(400, std::string("Bad Request")));
    http_errors.insert(std::make_pair(403, "Forbidden"));
    http_errors.insert(std::make_pair(404, "Not Found"));
    http_errors.insert(std::make_pair(405, "Method Not Allowed"));
    http_errors.insert(std::make_pair(408, "Request Timeout"));
    http_errors.insert(std::make_pair(410, "Gone"));
    http_errors.insert(std::make_pair(411, "Length Required"));
    http_errors.insert(std::make_pair(414, "Request URI too long"));
    http_errors.insert(std::make_pair(451, "Unavailable For Legal Reasons"));
    // 500 error codes
    http_errors.insert(std::make_pair(500, "Internal Server Error"));
    http_errors.insert(std::make_pair(505, "HTTP Version Not Supported"));
    http_errors.insert(std::make_pair(507, "Insufficient Storage"));
}

GlobalStorage& GlobalStorage::get()
{
    if (instance == nullptr)
        instance = new GlobalStorage();
    return *instance;
}