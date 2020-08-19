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

using std:: vector;
using std::ifstream;
using std::getline, std::replace;
using std:: istringstream;
using std::cout, std::cerr;
using std::string;
using std::array;
using std::unique_ptr;

string utils::get_value_sysfile(string filepath, string key,vector<char> separators){
	ifstream os_file(filepath);
	string line;
	while(getline(os_file,line)){
		replace(line.begin(),line.end(), ' ', '_');
		for(auto sep:separators){
			replace(line.begin(),line.end(), sep, ' ');
		}
		istringstream line_stream(line);
		string k,val;
		while(line_stream >> k >> val){
			if(k == key){
				replace(val.begin(), val.end(), '_', ' ');
				return val;
			}
		}
	}
	return "None";
}

string utils::remove_char_str(string str, char ch){
	str.erase(std::remove(str.begin(), str.end(), ch), str.end());
	return str;
}

string utils::execute(const char *cmd){
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&CLOSE)> pipe(OPEN(cmd, "r"), CLOSE);
    if (!pipe) {
        cerr<<("popen() failed!\n");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

double utils::round_n(double x, size_t n){
	return roundf(x * pow(10,n) / pow(10,n)) ;
}
