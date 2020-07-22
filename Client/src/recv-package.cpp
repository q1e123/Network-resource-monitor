#include "recv-package.h"

#include <iostream>
#include <sstream>

Recv_Package::Recv_Package(std::string package){
	std::istringstream iss_package(package);
	std::string actives;
	getline(iss_package, actives, '@');
	std::istringstream iss_actives(actives);
	std::string user_data_str;
	getline(iss_actives, user_data_str, ';');
	while(getline(iss_actives, user_data_str, '|')){
		std::string user, data;
		std::istringstream iss_user_data(user_data_str);
		getline(iss_user_data, user, ';');
		getline(iss_user_data, data);
		user_data[user] = data;	
	}
	std::string inactive;
	getline(iss_package,inactive);
	std::istringstream iss_inactive(inactive);
	std::string user;
	getline(iss_inactive, user, ';');
	while(getline(iss_inactive, user, ';')){
		inactive_users.push_back(user);
	}
}

std::map<std::string, std::string> Recv_Package::get_user_data(){
	return user_data;
}

std::vector<std::string> Recv_Package::get_inactive_users(){
	return inactive_users;
}
