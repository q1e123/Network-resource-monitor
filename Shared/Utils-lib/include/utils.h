#ifndef UTILS_H
#define UTILS_H

#include <cmath>

#include <vector>
#include <string>

namespace utils{
	std::string get_value_sysfile(std::string filepath, std::string key,std::vector<char> separators);
	std::string remove_char_str(std::string str, char ch);
	std::string execute(const char *cmd);
	double round_n(double x, size_t n);
};

#endif
