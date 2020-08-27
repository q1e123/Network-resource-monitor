#include "system.h"

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
}

System::~System(){
	delete this->logger;
}

string System::get_os(){
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

map<string, double> System::get_cpu_usage(){
	return OS::get_cpu_usage();
}

void System::update_cpu_usage(){
	cpu_usage = OS::get_cpu_usage();
}

string System::get_ip(){
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

map<string, Network_Usage> System::get_network_usage(){
	return network_usage;
}

void System::update_network_usage(){
	network_usage = OS::get_network_usage();
}

vector<string> System::get_network_interfaces(){
	return network_interfaces;
}

vector<Process> System::get_process_list(){
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
	pkg += ";" + std::to_string(this->timestamp);
	for(auto user : user_list){
		pkg += user + ":";
	}
	pkg.pop_back();
	return pkg;
}

System::System(std::string serialization){
	std::istringstream iss(serialization);	
	string tmp;
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
					cpu_usage[cpu_name] = std::stod(usage);
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
				while (getline(user_list_iss, user, ':')){
					user_list.push_back(user);
				}
			}
				
		}
		++pos;
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

std::vector<std::string> System::get_user_list(){
	return this->user_list;
}

void System::update_user_list(){
	user_list = OS::get_user_list();
}