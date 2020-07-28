#ifndef CLIENT_INFO_H
#define CLIENT_INFO_H



#ifdef __linux__
#include <arpa/inet.h>
typedef int SOCKET;
#elif defined _WIN32 || defined _WIN64

#include <winsock2.h>
#include <Ws2tcpip.h>
#endif

#include <thread>
#include <cstdio>
#include <string>

class Client_Info {
public:
	Client_Info();
	Client_Info(size_t socket_number, char* ip);
	size_t get_socket_number();
	void set_user(std::string user);
	std::string get_user();
	char* get_ip();
private:
	SOCKET socket_number;
	std::string user;
	char* ip;
};

#endif
