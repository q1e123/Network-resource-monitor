#include "network-package.h"

std::string Network_Package::send_package(std::string serialization, std::string user){
	std::string tmp;
	tmp = "0;" + user + ";" + serialization;
	return tmp;
}
