#include <vector>
#include <sstream>
#include "tools.hpp"

std::string cleanpath(std::string const &path)
{
    std::vector<std::string> arr = split(path, "/");
    std::vector<std::string> arr2;
    std::stringstream ss;
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
    for (size_t i = 0; i < arr2.size(); i++)
    {
        if (i == 0 && arr2[i][0] != '/')
            ss << "/";
        ss << (i == 0 ? "" : "/") << arr2[i];
    }
    return ss.str() == "" ? "/" : ss.str();
}

std::string joinpath(std::string const &path1 , std::string const &path2)
{
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

