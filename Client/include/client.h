#ifndef CLIENT_H
#define CLIENT_H

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
typedef int SOCKET;
#define STRCAT strcat
#elif defined _WIN32 || defined _WIN64
#define STRCAT strcat_s
#include "msw.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCKAPI_
#pragma comment( lib, "ws2_32.lib")
#include <winsock2.h>
#include <Ws2tcpip.h>
#endif


#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <thread>
#include <mutex>
#include <iostream>
#include <string>

#include "logger.h"
#include "system.h"
#include "database-structs.h"

#define LOOPBACK_ADDR	"127.0.0.1"
#define DEFAULT_PORT 50005
#define MESSAGE_SIZE 500

class Server_Down_Exception: public std::exception{
public:
    virtual const char* what() const throw();
private:
};

class Login_Exception: public std::exception{
public:
    virtual const char* what() const throw();
private:
};

class Client {
public:
	Client();
	Client(std::string user, std::string machine_id);
	void start();
	void send_message(std::string message);
	void connect_to_server();
	void start_reciver();

	std::string message_recived;
	~Client();
	std::string get_message_recived();
	std::string get_server_name();
	std::string get_user();
	std::string get_role();
	std::vector<System*> get_active_systems();
	std::vector<std::string> get_inactive_systems();
	std::vector<DB_Users> get_users();

	void request_active_systems();
	void request_inactive_systems();
	void request_users();

	void send_system_state(System *system);
	void send_log_file(Logger *logger);

private:
	std::string server_name;
	std::string role;
	std::string machine_id;
	std::string ip_std;
	std::vector<System*> active_systems;
	std::vector<std::string> inactive_systems;
	std::vector<DB_Users> users;
	
	void recive_message();
	int socket_init();
	int socket_quit();
	bool socket_check(SOCKET socket);
	int socket_close(SOCKET socket);
	std::thread reciver;
	std::mutex server_name_mutex, message_recived_mutex;
	struct sockaddr_in server_addr;
	SOCKET client_sock;
	SOCKET server_sock;
	int server_addr_size;
	int port_number;
	std::string username;
	char *ip[INET_ADDRSTRLEN];

	Logger *logger;

	void init();

	void run_commannd(std::string command);
	void run_get_systems_active(size_t number_of_systems);
	void run_get_systems_inactive(size_t number_of_systems);
	void run_get_users(size_t number_of_users);
};

#endif
