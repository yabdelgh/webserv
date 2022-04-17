#include "Parser.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
using namespace std;
 
int main()
{
    string str = "Tokenize a string in C++";
 
    vector<string> tokens;
 
    istringstream iss(str);
    string s;
    while (iss >> s) {
        tokens.push_back(s);
    }
 
    for (const string &s: tokens) {
        cout << s << endl;
    }
 
    return 0;
}