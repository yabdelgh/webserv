#include <string>
#include <iostream>
#include "./config_parser/regex/defined_patterns.hpp"

int main() {
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

    unordered_map<string, Pattern> patterns = get_patterns();

    string g_ip = " good ip";
    string b_ip = " bad ip";
    string ip = "10.10.10.10";
    cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    ip = "000.0.0.0";
    cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    ip = "   000.000.000.000   ";
    cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    ip = "0.0.0.0";
    cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    ip = ".000.000.000";
    cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    ip = "0.0.0.0000";
    cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    ip = "   0.0.0.00.00   ";
    cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
    ip = "   0.0.0    ";
    cout << ip << (patterns.find("ip")->second.match(ip) ? g_ip : b_ip) << endl;
}