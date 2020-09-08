#include "system.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <fstream>

System::System(){
	os = OS::get_os();
	total_ram = OS::get_total_ram();
	avalabile_ram = OS::get_avalabile_ram();
	cpu_usage = OS::get_cpu_usage();
	network_usage = OS::get_network_usage();
	network_interfaces = OS::get_network_interfaces();
	process_list = OS::get_process_list();
	machine_id = OS::get_machine_id();
	current_user = OS::get_current_user();
	user_list = OS::get_user_list();
	environment_variables = OS::get_environment_variables();
	avalabile_space = OS::get_avalabile_space();
	installed_programs = OS::get_installed_programs();
	ipv4 = OS::get_ipv4_map();
}

System::~System(){
	delete this->logger;
}

std::string System::get_os(){
	return os;
}
size_t System::get_total_ram(){
	return total_ram;
}

size_t System::get_avalabile_ram(){
	return avalabile_ram;
}

void System::update_avalabile_ram(){
	avalabile_ram = OS::get_avalabile_ram();
	used_ram = total_ram - avalabile_ram;
}

std::map<std::string, double> System::get_cpu_usage(){
	return OS::get_cpu_usage();
}

void System::update_cpu_usage(){
	cpu_usage = OS::get_cpu_usage();
}

std::string System::get_ip(){
	return ip;
}

void System::shutdown(){
	OS::shutdown();
}

void System::restart(){
	OS::restart();
}

size_t System::get_used_ram(){
	return used_ram;
}

std::map<std::string, Network_Usage> System::get_network_usage(){
	return network_usage;
}

void System::update_network_usage(){
	network_usage = OS::get_network_usage();
}

std::vector<std::string> System::get_network_interfaces(){
	return network_interfaces;
}

std::vector<Process> System::get_process_list(){
	return process_list;
}

void System::update_process_list(){
	process_list = OS::get_process_list();
}

std::string System::serilize(){
	std::string pkg = "";
	pkg += os + ";";
	pkg += std::to_string(total_ram) + ";";
	pkg += std::to_string(avalabile_ram) + ";";
	pkg += std::to_string(used_ram)+ ";";
	for(auto item : cpu_usage){
		pkg += item.first + ":" + std::to_string(item.second).substr(0, std::to_string(item.second).size() - 4) + "-";
	}
	pkg.pop_back();
	pkg += ";";
	for(auto item : network_usage){
		std::string tx = std::to_string(item.second.get_tx());
		std::string rx = std::to_string(item.second.get_rx());

		pkg += item.first + ":" + rx.substr(0, rx.size() - 4) + ":" +tx.substr(0, tx.size() - 4) + "|";

	}
	pkg.pop_back();	
	pkg += ";" + this->current_user;
	timestamp = std::time(0);
	pkg += ";" + std::to_string(this->timestamp) + ";";
	
	for(auto user : user_list){
		pkg += user.username + ":" + std::to_string(user.last_login) + "|";
	}
	pkg.pop_back();
	pkg += ";" + std::to_string(this->avalabile_space) + ";";

	for(auto program : installed_programs){
		pkg += program + ":";
	}
	pkg.pop_back();
	pkg += ";";

	for(auto item : ipv4){
		pkg += item.first + ":" + item.second + "|";
	}
	pkg.pop_back();
	pkg += ";";

	for(auto item : environment_variables){
		pkg += item.first + "\t" + item.second + "#";
	}
	pkg.pop_back();
	return pkg;
}

System::System(std::string serialization){
	std::istringstream iss(serialization);	
	std::string tmp;
	size_t pos = 0;
	while(getline(iss, tmp, ';')){
		switch(pos){
			case 0:{
				os = tmp;
				break;
			}
			case 1:{
				try{
					total_ram = std::stol(tmp);

				}catch(const std::exception& e) {
					std::cerr << "System serialization error: " << e.what() << " total ram = |" << tmp <<"|" <<  std::endl;
				}
				
				break;
			}
			case 2:{
				try{
					avalabile_ram = std::stol(tmp);
				}catch(const std::exception& e) {
					std::cerr << "System serialization error: " << e.what() << " avalabile ram = |" << tmp <<"|" <<  std::endl;
				}	
				break;
			}
			case 3:{
				try{
					used_ram = std::stol(tmp);
				}catch(const std::exception& e) {
					std::cerr << "System serialization error: " << e.what() << " used ram = |" << tmp <<"|" <<  std::endl;
				}
				break;
			}				
			case 4:{
				std::istringstream proc_list_iss(tmp);
				std::string cpu_line;
				while(getline(proc_list_iss, cpu_line, '-')){
					std::istringstream proc_iss(cpu_line);
					std::string cpu_name, usage;
					getline(proc_iss, cpu_name, ':');
					getline(proc_iss, usage, ':');
					try{
						cpu_usage[cpu_name] = std::stod(usage);
					}catch(const std::exception& e)
					{
						cpu_usage[cpu_name] = 0;
						std::cerr << "System - cpu usage " << e.what() << " " << usage << std::endl;
					}
					
				}
				break;
			}
			case 5:{
				std::istringstream network_list_iss(tmp);
				std::string network_line;
				while(getline(network_list_iss, network_line, '|')){
					std::istringstream network_iss(network_line);
					std::string network_interface, usage_rx, usage_tx;
					getline(network_iss, network_interface, ':');
					getline(network_iss, usage_rx, ':');
					getline(network_iss, usage_tx, ':');
					try{
						Network_Usage usage(std::stol(usage_rx), std::stol(usage_tx));
						network_usage[network_interface] = usage;
					}catch(const std::exception& e) {
						Network_Usage usage(0, 0);
						network_usage[network_interface] = 0;
						std::cerr << "System serialization error: " << e.what() << " Network usage rx = |"
						 << usage_rx <<"| tx = |" << usage_tx << "|" << std::endl;
					}
				}
				break;
			}
			case 6:{
				this->current_user = tmp;
				break;
			}
			case 7:{
				this->timestamp = std::stol(tmp);
				break;
			}
			case 8:{
				std::istringstream user_list_iss(tmp);
				std::string user;
				while (getline(user_list_iss, user, '|')){
					std::istringstream user_iss(user);
					std::string username, lastlog_str;
					getline(user_iss, username, ':');
					getline(user_iss, lastlog_str);
					System_User sys_user;
					sys_user.username = username;
					try
					{
						
					
					sys_user.last_login = std::stol(lastlog_str);
					user_list.push_back(sys_user);
					}
					catch(const std::exception& e)
					{
						std::cout << serialization << std::endl <<std::endl;
					}
				}
				break;
			}
			case 9:{
				try{
					avalabile_space = std::stol(tmp);
				}catch(const std::exception& e) {
					std::cerr << "System serialization error: " << e.what() << " avalabile space = |" << tmp <<"|" <<  std::endl;
				}
				break;
			}
			case 10:{
				std::istringstream program_list_iss(tmp);
				std::string program;
				while (getline(program_list_iss, program, ':')){
					installed_programs.push_back(program);
				}
				break;
			}
			case 11:{
				std::istringstream network_ipv4_iss(tmp);
				std::string network_interface;
				while (getline(network_ipv4_iss, network_interface, '|')){
					std::istringstream network_interface_iss(network_interface);
					std::string net_interface, ip;
					getline(network_interface_iss, net_interface, ':');
					getline(network_interface_iss, ip);
					ipv4[net_interface] = ip;
				}
				break;
			}
		}
		++pos;
	}

	std::istringstream environment_variables_list(tmp);
	std::string item;
	while (getline(environment_variables_list, item, '#')){
		std::string key;
		std::istringstream item_iss(item);
		getline(item_iss, key, '\t');
		std::string value;
		getline(item_iss, value);
		environment_variables[key] = value;
	}
}

std::string System::get_machine_id(){
	return this->machine_id;
}

std::string System::get_current_user(){
	return this->current_user;
}

bool System::sanity_check(){
	if(total_ram < avalabile_ram + used_ram){
		return false;
	}
	for(auto item : cpu_usage){
		if(item.second > 100 || item.second < 0){
			return false;
		}
	}
	return true;
}

std::time_t System::get_timestamp(){
	return this->timestamp;
}

void System::log_init(){
	logger = new Logger(this->system_log_file);
	logger->init("../Init/system-logger.ini");
}

void System::log(){
	logger->add_system(serilize());
}

std::vector<System_User> System::get_user_list(){
	return this->user_list;
}

void System::update_user_list(){
	user_list = OS::get_user_list();
}

std::map<std::string, std::string> System::get_environment_variables(){
	return this->environment_variables;
}

void System::update_environment_variables(){
	this->environment_variables = OS::get_environment_variables();
}

double System::get_avalabile_space(){
	return this->avalabile_space;
}

void System::update_avalabile_space(){
	avalabile_space = OS::get_avalabile_space();
}

std::vector<std::string> System::get_installed_programs(){
	return this->installed_programs;
}

void System::update_installed_programs(){
	installed_programs = OS::get_installed_programs();
}

bool System::check_if_installed(std::string software){
	std::cout << "|" << software << "|" << installed_programs[0] <<"|" << std::endl;
	if(std::find(installed_programs.begin(), installed_programs.end(), software) != installed_programs.end()){
		return true;
	}
	return false;
}

void System::create_error_log(){
	OS::create_error_log();
}

std::map<std::string, std::string> System::get_ipv4(){
	return ipv4;
}

void System::update_ipv4(){
	ipv4 = OS::get_ipv4_map();
}