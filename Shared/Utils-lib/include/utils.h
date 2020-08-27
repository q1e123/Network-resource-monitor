#ifndef UTILS_H
#define UTILS_H

#include <cmath>

#include <vector>
#include <string>

#include "build_opts_utils.h"

namespace utils{
	std::string UTILS_IMPEXP get_value_sysfile(std::string filepath, std::string key,std::vector<char> separators);
	std::string UTILS_IMPEXP remove_char_str(std::string str, char ch);
	std::string UTILS_IMPEXP execute(const char *cmd);
	double UTILS_IMPEXP round_n(double x, size_t n);
};

#endif
