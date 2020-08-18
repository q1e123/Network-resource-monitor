#include "system.h"
#include <stdexcept>      // std::out_of_range

#include <sstream>

System::System(){
	os = OS::get_os();
	total_ram = OS::get_total_ram();
	avalabile_ram = OS::get_avalabile_ram();
	cpu_usage = OS::get_cpu_usage();
	network_usage = OS::get_network_usage();
	network_interfaces = OS::get_network_interfaces();
	process_list = OS::get_process_list();
	machine_id = OS::get_machine_id();
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
	return pkg;
}

System::System(std::string serialization){
	std::istringstream iss(serialization);	
	string tmp;
	size_t pos = 0;
	while(getline(iss, tmp, ';')){
		switch(pos){
			case 0:
				os = tmp;
				break;
			case 1:
				total_ram = std::stol(tmp);
				break;
			case 2:
				avalabile_ram = std::stol(tmp);
				break;
			case 3:
				used_ram = std::stol(tmp);
				break;
			case 4:
				std::istringstream proc_list_iss(tmp);
				std::string cpu_line;
				while(getline(proc_list_iss, cpu_line, '-')){
					std::istringstream proc_iss(cpu_line);
					std::string cpu_name, usage;
					getline(proc_iss, cpu_name, ':');
					getline(proc_iss, usage, ':');
					cpu_usage[cpu_name] = std::stod(usage);
				}
		}
		++pos;
	}
}

std::string System::get_machine_id(){
	return this->machine_id;
}