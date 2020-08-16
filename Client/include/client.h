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
#endif


#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <thread>
#include <mutex>
#include <iostream>
#include <string>

#include "logger.h"

#define LOOPBACK_ADDR	"127.0.0.1"
#define DEFAULT_PORT 50005
#define MESSAGE_SIZE 500
class Client {
public:
	Client();
	Client(std::string user, std::string server_ip, size_t socket);
	void start();
	void send_message(std::string message);
	void connect_to_server();
	void start_reciver();

	std::string role;
	std::string message_recived;
	~Client();
	std::string get_message_recived();
	std::string get_server_name();
	std::string get_user();
private:
	std::string server_name;
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
};

#endif
