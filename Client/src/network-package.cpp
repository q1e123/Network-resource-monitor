#include "network-package.h"

std::string Network_Package::send_package(std::string serialization){
	std::string tmp;
	tmp = "s;" + serialization;
	return tmp;
}
