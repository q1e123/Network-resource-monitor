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
#include <iostream>
#include <string>

#define LOOPBACK_ADDR	"127.0.0.1"
#define DEFAULT_PORT 50005
class Client{
public:
	Client();
	Client(std::string user, size_t socket);
	void start();
	void send(char *msg);
	~Client();
private:
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
	
	std::thread worker;
	void recv_msg(size_t sock);

};

#endif
