#ifdef __linux__

#ifndef Linux_H
#define Linux_H
#include <fstream>
#include <string>
#include <map>
#include <cstdio>

#include "jiffy.h"

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
};

#endif
#endif
