#ifndef System_H
#define System_H

#include <string>
#include <cstdio>
#include <map>

#ifdef __linux__
	#include "linux.h"
	#define OS Linux
#elif defined _WIN32 || defined _WIN64
	#include "msw.h"
	#define OS Msw
#endif

using std::string;
using std::map;
class System{
public:
	System();
	
	size_t get_total_ram();
	size_t get_avalabile_ram();
	size_t get_used_ram();

	string get_os();
	void update_avalabile_ram();

	map<string, double> get_cpu_usage();
	void update_cpu_usage();

	void shutdown();
	void restart();

	string get_ip();
private:
	size_t total_ram, avalabile_ram, used_ram;
	string os, ip;
	map<string, double> cpu_usage;
	
};
#endif
