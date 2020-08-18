#ifndef System_H
#define System_H

#include <string>
#include <vector>
#include <cstdio>
#include <map>
#include "network-usage.h"
#include "my-process.h"

#ifdef __linux__
	#define OS Linux
	#include "linux.h"
#elif defined _WIN32 || defined _WIN64
	#define OS Msw
	#include "msw.h"
#endif

using std::string;
using std::vector;
using std::map;
class System{
public:
	System();
	System(std::string serialization);
	
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

	vector<Process> get_process_list();
	void update_process_list();

	// os;total_ram;avalabile_ram;used_ram;<cpu_usage>
	// cpu_usage = cpu_name_0:usage_0-cpu_name_1:usage_1-...-cpu_name_n:usage_n
	std::string serilize();

	std::string get_machine_id();
private:
	size_t total_ram, avalabile_ram, used_ram;
	string os, ip;
	vector<string> network_interfaces;
	map<string, double> cpu_usage;
	map<string, Network_Usage> network_usage;
	vector<Process> process_list;
	std::string machine_id;
};
#endif
