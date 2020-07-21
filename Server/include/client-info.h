#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <arpa/inet.h>
#include <thread>
#include <cstdio>
#include <string>

class Client_Info{
public:
	Client_Info(size_t socket_number=0, char *ip="");
	size_t get_socket_number();
	void set_user(std::string user);
	std::string get_user();
	char *get_ip();
private:
	size_t socket_number;
	std::string user;
	char *ip;
};

#endif
