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
#include "my-process.h"
#include "system-structs.h"

namespace Linux{
	const std::string PROC_DIR = "/proc/";
	const std::string STAT_FILE = PROC_DIR +  "stat";
	const std::string CPUINFO_FILE = PROC_DIR +"cpuinfo";
	const std::string STATUS_FILE = PROC_DIR +"status";
	const std::string MEMINFO_FILE = PROC_DIR + "meminfo";
	const std::string OS_RELEASE_FILE = "/etc/os-release";
	
	std::string get_os();
	size_t get_total_ram();
	size_t get_avalabile_ram();
	std::map<std::string, double> get_cpu_usage();

	void shutdown();
	void restart();

	std::vector<std::string> get_network_interfaces();
	Network_Usage get_interface_usage(std::string interface);
	std::map<std::string, Network_Usage> get_network_usage();
	std::map<std::string, std::string> get_ipv4_map();
	std::string get_ivp4_for(std::string network_interface);

	void get_proc_info(size_t pid, Process &process);
	std::vector<size_t> get_pids();
	std::vector<Process> get_process_list();

	std::string get_machine_id();

	std::string get_current_user();

	std::vector<System_User> get_user_list();
	struct tm* get_last_login(std::string user);
	
	std::map<std::string, std::string> get_environment_variables();

	double get_avalabile_space();

	std::vector<std::string> get_installed_programs();

	void create_error_log();
};

#endif
#endif
