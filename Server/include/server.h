#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <sstream>

#include "client-info.h"

#define LOOPBACK_ADDR	"127.0.0.1"
#define DEFAULT_PORT 50005

class Server{
public:
	Server(std::string name, size_t port=DEFAULT_PORT);
	void start();
private:
size_t idx; 
	std::string name;	

	std::thread send_worker;

	std::mutex mtx;
	std::vector<Client_Info> clients;
	std::map<size_t, std::thread> workers;	
	std::map<std::string, std::string> systems;
	
	struct sockaddr_in my_addr, their_addr;
	int my_sock;
	int their_sock;
	socklen_t their_addr_size;
	int portno;
	char msg[500];
	int len;
	Client_Info cl;
	char ip[INET_ADDRSTRLEN];

	void send_to_all();
	void send_msg(char*msg, int curr);
	void recv_msg(Client_Info client);
	void run_cmd(std::string cmd);
	void cmd_sys(std::istringstream cmd);

};
#endif
