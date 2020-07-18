#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H

#include <arpa/inet.h>
#include <thread>
#include <cstdio>

class Client_Info{
public:
	Client_Info(size_t socket_number=0, char *ip="");
	size_t get_socket_number();
	char *get_ip();
private:
	size_t socket_number;
	char *ip;
};

#endif
