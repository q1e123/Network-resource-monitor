
#include "network-package.h"
#include <iostream>
std::string Network_Package::serilize_system(System system){
	std::string pkg = "s;";
	pkg += system.get_os() + ";";
	pkg += std::to_string(system.get_total_ram()) + ";";
	pkg += std::to_string(system.get_avalabile_ram()) + ";";
	pkg += std::to_string(system.get_used_ram()) + ";";
	for(auto item : system.get_cpu_usage()){
		pkg += item.first + ": " + std::to_string(item.second).substr(0, std::to_string(item.second).size() - 4) + ";";
	}
	return pkg;
}
