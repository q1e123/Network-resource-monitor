#ifndef CLIENT_H
#define CLIENT_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <thread>
#include <mutex>
#include <iostream>
#include <string>

#define LOOPBACK_ADDR	"127.0.0.1"
#define DEFAULT_PORT 50005
class Client{
public:
	Client();
	Client(std::string user, std::string server_ip, size_t socket);
	void start();
	void send_msg(std::string msg);
	std::string msg_rec, msg_send;
	void recv_msg();
	~Client();
	std::string get_msg_rec();
	std::string get_server_name();
	std::string get_user();
private:
	std::string username_str;
	std::string msg_old;
	std::thread worker_r, worker_s;
	std::mutex mtx;
	std::string server_name;
	struct sockaddr_in their_addr;
	int my_sock;
	int their_sock;
	int their_addr_size;
	int portno;
	char msg[500];
	char *username;
	char res[600];
	char ip[INET_ADDRSTRLEN];
	int len;
};

#endif
