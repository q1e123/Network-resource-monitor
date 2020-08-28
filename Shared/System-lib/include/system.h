#ifndef System_H
#define System_H

#include <ctime>

#include <string>
#include <vector>
#include <cstdio>
#include <map>
#include "network-usage.h"
#include "my-process.h"

#include "logger.h"

#include "build-opts-system.h"

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

class SYSTEM_IMPEXP System{
public:
	System();
	System(std::string serialization);
	~System();

	const std::string system_log_file = "system-logger.txt";
	Logger *logger;

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

	// os;total_ram;avalabile_ram;used_ram;<cpu_usage>;<network_usage>
	// cpu_usage = cpu_name_0:usage_0-cpu_name_1:usage_1-...-cpu_name_n:usage_n
	// network_usage = network_interface_0:usage_rx_0:usage_tx_0-network_interface_1:usage_rx_1:usage_tx_1--...-network_interface_n:usage_rx_n:usage_tx_n
	std::string serilize();

	std::string get_machine_id();

	std::string get_current_user();

	std::time_t get_timestamp();

	bool sanity_check();

	void log_init();
	void log();
	std::string get_log();

	std::vector<std::string> get_user_list();
	void update_user_list();

	std::map<std::string, std::string> get_environment_variables();
	void update_environment_variables();

	double get_avalabile_space();
	void update_avalabile_space();
	
private:
	size_t total_ram, avalabile_ram, used_ram;
	double avalabile_space;
	string os, ip, current_user;
	vector<string> network_interfaces;
	map<string, double> cpu_usage;
	map<string, Network_Usage> network_usage;
	vector<Process> process_list;
	std::string machine_id;
	std::vector<std::string> user_list;
    std::time_t timestamp;
	std::map<std::string, std::string> environment_variables;
};
#endif
