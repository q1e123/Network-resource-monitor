#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <cmath>
#include <vector>
using std::string;
using std::vector;
namespace utils{
	string get_value_sysfile(string filepath, string key,vector<char> separators);
	string remove_char_str(string str, char ch);
	string execute(char *cmd);
	double round_n(double x, size_t n);
};

#endif
