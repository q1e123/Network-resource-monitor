#ifdef __linux__
	#define OPEN popen
	#define CLOSE pclose
#elif defined _WIN32 || defined _WIN64
	#define OPEN _popen
	#define CLOSE _pclose
#endif

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>
#include <fstream>
#include <vector>
#include "utils.h"

std::string utils::get_value_sysfile(std::string filepath, std::string key,std::vector<char> separators){
	std::ifstream os_file(filepath);
	std::string line;
	while(getline(os_file,line)){
		std::replace(line.begin(),line.end(), ' ', '_');
		for(auto sep:separators){
			std::replace(line.begin(),line.end(), sep, ' ');
		}
		std::istringstream line_stream(line);
		std::string k,val;
		while(line_stream >> k >> val){
			if(k == key){
				std::replace(val.begin(), val.end(), '_', ' ');
				return val;
			}
		}
	}
	return "None";
}

std::string utils::remove_char_str(std::string str, char ch){
	str.erase(std::remove(str.begin(), str.end(), ch), str.end());
	return str;
}

std::string utils::execute(const char *cmd){
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&CLOSE)> pipe(OPEN(cmd, "r"), CLOSE);
    if (!pipe) {
        std::cerr<<("popen() failed!\n");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

double utils::round_n(double x, size_t n){
	return roundf(x * pow(10,n) / pow(10,n)) ;
}
