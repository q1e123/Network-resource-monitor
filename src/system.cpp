#include "system.h"


System::System(){
	os = OS::get_os();
	total_ram = OS::get_total_ram();
	avalabile_ram = OS::get_avalabile_ram();
	cpu_usage = OS::get_cpu_usage();
	ip = OS::get_ip();
	network_usage = OS::get_network_usage();
	network_interfaces = OS::get_network_interfaces();
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
