#ifdef _WIN32 || _WIN64
#ifndef MSW_H
#define MSW_H

#include <ctime>

#include <vector>
#include <fstream>
#include <string>
#include <map>
#include "network-usage.h"
#include "my-process.h"
#include <windows.h>

#include "system-structs.h"

namespace Msw{
	std::string get_os();
	
	size_t get_total_ram();
	size_t get_avalabile_ram();
	std::map<std::string,double> get_cpu_usage();
	
	void shutdown();
	void restart();
	
	std::vector<std::string> get_network_interfaces();
	std::map<std::string, Network_Usage> get_network_usage();
	
	void get_proc_info(DWORD pid, Process &process);
	std::vector<Process> get_process_list();

	std::string get_machine_id();

	std::string get_current_user();

	std::vector<System_User> get_user_list();
	std::time_t get_last_login(std::wstring user);

	std::map<std::string, std::string> get_environment_variables();

	double get_avalabile_space();

	std::vector<std::string> get_drive_list();

	std::vector<std::string> get_installed_programs();

	std::map<std::string, std::string> get_ipv4_map();

	void create_error_log();
	void create_application_log();
	void create_security_log();
	void create_setup_log();
	void create_system_log();
};

#endif
#endif
