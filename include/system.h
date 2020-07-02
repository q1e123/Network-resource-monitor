#ifndef System_H
#define System_H

#include <string>
#include <vector>
#include <cstdio>
#include <map>
#include "network-usage.h"

#ifdef __linux__
	#include "linux.h"
	#define OS Linux
#elif defined _WIN32 || defined _WIN64
	#include "msw.h"
	#define OS Msw
#endif

using std::string;
using std::vector;
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

	map<string, Network_Usage> get_network_usage();
	void update_network_usage();

	vector<string> get_network_interfaces();
private:
	size_t total_ram, avalabile_ram, used_ram;
	string os, ip;
	vector<string> network_interfaces;
	map<string, double> cpu_usage;
	map<string, Network_Usage> network_usage;
};
#endif
