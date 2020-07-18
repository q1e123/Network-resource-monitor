#include "client.h"

Client::Client(std::string user, size_t sock){
	username = const_cast<char*>(user.c_str());
	portno = sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(portno);
	their_addr.sin_addr.s_addr = inet_addr(LOOPBACK_ADDR);

	if (connect(my_sock, (struct sockaddr *)&their_addr, sizeof(their_addr)) < 0) {
		std::cerr<< "connection not established\n";
		exit(1);
	}

	inet_ntop(AF_INET, (struct sockaddr *)&their_addr, ip, INET_ADDRSTRLEN);
	std::cout << "connected to " << ip << " \n";

	len = write(my_sock, username, strlen(username));
	if (len < 0) {
		std::cerr << "initial identity message not sent\n";
		exit(1);
	}
	worker = std::thread(&Client::recv_msg, this, my_sock);
}

void Client::recv_msg(size_t sock)
{
	int their_sock = sock;
	char msg[500];
	int len;
	while ((len = recv(their_sock, msg, 500, 0)) > 0) {

		msg[len] = '\0';
		fputs(msg, stdout);
		memset(msg, '\0', sizeof(msg));
	}
}

Client::Client(){
}
Client::~Client(){
	worker.join();
	close(my_sock);
}

void Client::send(char *msg){
	strcpy(res, username);
	strcat(res, ":");
	strcat(res, msg);
	len = write(my_sock, res, strlen(res));
	if (len < 0) {
		std::cerr << "message not sent\n";
		exit(1);
	}
	memset(msg, '\0', sizeof(msg));
	memset(res, '\0', sizeof(res));
}
