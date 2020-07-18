#ifdef __linux__

#ifndef Linux_H
#define Linux_H
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>

#include "jiffy.h"
#include "network-usage.h"
#include "process.h"

using std::vector;
using std::string;
using std::map;

namespace Linux{
	const string PROC_DIR = "/proc/";
	const string STAT_FILE = PROC_DIR +  "stat";
	const string CPUINFO_FILE = PROC_DIR +"cpuinfo";
	const string STATUS_FILE = PROC_DIR +"status";
	const string MEMINFO_FILE = PROC_DIR + "meminfo";
	const string OS_RELEASE_FILE = "/etc/os-release";
	
	string get_os();
	size_t get_total_ram();
	size_t get_avalabile_ram();
	map<string, double> get_cpu_usage();

	void shutdown();
	void restart();

	string get_ip();
	vector<string> get_network_interfaces();
	Network_Usage get_interface_usage(string interface);
	map<string, Network_Usage> get_network_usage();
	
	void get_proc_info(size_t pid, Process &process);
	vector<size_t> get_pids();
	vector<Process> get_process_list();
};

#endif
#endif
