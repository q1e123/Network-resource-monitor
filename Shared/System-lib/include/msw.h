#ifdef _WIN32 || _WIN64
#ifndef MSW_H
#define MSW_H
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "network-usage.h"
#include "my-process.h"
#include <windows.h>
using std::string;
using std::map;
using std::vector;

namespace Msw{
	string get_os();
	
	size_t get_total_ram();
	size_t get_avalabile_ram();
	map<string,double> get_cpu_usage();
	
	void shutdown();
	void restart();
	
	vector<string> get_network_interfaces();
	map<string, Network_Usage> get_network_usage();
	
	void get_proc_info(DWORD pid, Process &process);
	vector<Process> get_process_list();

	std::string get_machine_id();

	std::string get_current_user();

	std::vector<std::string> get_user_list();
};

#endif
#endif
