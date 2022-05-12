#include <vector>
#include <sstream>
#include <iostream>
#include "tools.hpp"

std::string cleanpath(std::string const &path)
{
    std::vector<std::string> arr = split(path, "/");
    std::vector<std::string> arr2;
    std::stringstream ss;
    const char c = path[0] == '/' ? '/' : '\0';

    for (size_t i = 0; i < arr.size(); i++)
    {
        if (arr[i] == "..")
        {
            if (arr2.size() > 0)
                arr2.pop_back();
        }
        else if (arr[i] != ".")
            arr2.push_back(arr[i]);
    }
    ss << c;
    for (size_t i = 0; i < arr2.size(); i++)
        ss << (i == 0 ? "" : "/") << arr2[i];
    return ss.str();
}

std::string joinpath(std::string const &path1 , std::string const &path2)
{
    std::cout << path1 << " " << path2 << std::endl;
    if (!path1.empty() && !path2.empty())
        if (path1.back() != '/' && path2.front() != '/')
            return path1 + "/" + path2;
    return path1 + path2;
}

std::string extension(std::string const &file_name)
{
    size_t pos = file_name.find_last_of(".");
    if (pos != -1)
        return file_name.substr(pos + 1);
    return "";
}

