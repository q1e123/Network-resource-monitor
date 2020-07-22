#ifndef RECV_PACKAGE_H
#define RECV_PACKAGE_H

#include <vector>
#include <string>
#include <map>

class Recv_Package{
public:
	Recv_Package(std::string package);
	std::map<std::string, std::string> get_user_data();
	std::vector<std::string> get_inactive_users();
private:
	std::vector<std::string> inactive_users;	
	std::map<std::string, std::string> user_data;
};

#endif
