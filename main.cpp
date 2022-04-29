#include <string>
#include <iostream>
#include "defined_patterns.hpp"
#include "tools.hpp"

int main(int ac, char **av) {
    // vector<string> tokens;
    // string ptrn("[11][22][44][[][]");
    // size_t idx = 0;
    // while (idx < ptrn.size())
    // {
    //     size_t start = ptrn.find('[', idx);
    //     if (start == -1)
    //         throw runtime_error("invalid pattern at " + to_string(idx));

    //     size_t end = ptrn.find(']', idx);
    //     end = (end == -1 ? ptrn.size() - 1 : end);
    //     idx = end + 1;
    //     tokens.push_back(ptrn.substr(start + 1, start - end));
    //     cout << ptrn.substr(start + 1, end - (start + 1)) << endl;
    // }

    // unordered_map<string, Pattern> patterns = get_patterns();

    // string g_ip = " good ip";
    // string b_ip = " bad ip";
    // string ip = "10.10.10.10";
    // cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    // ip = "000.0.0.0";
    // cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    // ip = "   000.000.000.000   ";
    // cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    // ip = "0.0.0.0";
    // cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    // ip = ".000.000.000";
    // cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    // ip = "0.0.0.0000";
    // cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    // ip = "   0.0.0.00.00   ";
    // cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    // ip = "   0.0.0    ";
    // cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    if (ac > 1)
    {
        std::string content;
        get_file_contents(av[1], content);
        std::cout << content << std::endl;
        // IParseable &server_config = *get_server_config();
        // size_t idx = 0;
        // std::cout << (server_config.parse(content, idx) ? "good" : "bad") << std::endl;
        // std::cout << "servers: " <<  server_config["server"][1]["listen"].size() << std::endl;
        // std::cout << "port:" << server_config["server"][0]["listen"][0]["host"].get_string() << std::endl;

        IParseable &rh = *get_request_header();
        size_t idx = 0;
        std::cout << (rh.cont_parse(content, idx) ? "good" : "bad") << std::endl;
        // std::cout << "servers: " <<  rh["server"][1]["listen"].size() << std::endl;
        // std::cout << "port:" << rh["server"][0]["listen"][0]["host"].get_string() << std::endl;
    }
}