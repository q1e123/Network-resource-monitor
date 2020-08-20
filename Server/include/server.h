#ifndef SERVER_H
#define SERVER_H

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
typedef int SOCKET;
#elif defined _WIN32 || defined _WIN64
#pragma comment( lib, "ws2_32.lib")
#include <winsock2.h>
#include <Ws2tcpip.h>
#endif

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <sstream>

#include "client-info.h"
#include "logger.h"
#include "database-manager.h"
#include "system.h"

#define LOOPBACK_ADDR	"127.0.0.1"
#define DEFAULT_PORT 50005

class Server {
public:
	Server(std::string name, size_t port = DEFAULT_PORT);
	void start();
private:
	std::string name;

	std::thread send_worker;

	std::mutex mtx;
	std::vector<Client_Info> clients;
	std::string disc_users;
	std::map<size_t, std::thread> workers;
	std::map<std::string, std::string> systems;

	struct sockaddr_in server_addr, client_addr;
	SOCKET server_sock;
	socklen_t client_addr_size;
	int port_number;
	char msg[500];
	int len;
	Client_Info cl;
	char ip[INET_ADDRSTRLEN];

	Logger *logger;
	Database_Manager database_manager;
	int socket_init();
	int socket_quit();
	bool socket_check(SOCKET socket);
	int socket_close(SOCKET socket);

	void send_to_all();
	void send_to(Client_Info client, std::string message);
	void recv_msg(Client_Info client);
	void remove_user(std::string user);

	void run_cmd(std::string cmd);
	void cmd_sys(std::string serialization);
	void cmd_log(std::string user, size_t number_of_logs);

	size_t find_client(std::string user);

};

class Sanity_Check_Failed_Exception: public std::exception{
public:
    virtual const char* what() const throw();
private:
};
#endif
