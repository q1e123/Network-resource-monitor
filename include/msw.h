#ifdef _WIN32 || _WIN64
#ifndef MSW_H
#define MSW_H
#include <fstream>
#include <string>
#include <map>
using std::string;
using std::map;
namespace Msw{
	string get_os();
	size_t get_total_ram();
	size_t get_avalabile_ram();
	map<string,double> get_cpu_usage();
	void shutdown();
	void restart();
	string get_ip();
};

#endif
#endif
