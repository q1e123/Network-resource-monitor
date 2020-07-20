#include "client.h"

Client::Client(std::string user, size_t sock){
	username_str = user;
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
	worker_r = std::thread(&Client::recv_msg, this);
	//worker_s = std::thread(&Client::send_msg, this);
}


#include<iostream>
void Client::recv_msg(){
	char msg[500];
	int len;
	while ((len = recv(my_sock, msg, 500, 0)) > 0) {
		msg[len] = '\0';
		mtx.lock();
		std::cout<<msg<<"\n";
		msg_rec = msg;
		mtx.unlock();
		memset(msg, '\0', sizeof(msg));
	}
}

Client::Client(){
}
Client::~Client(){
	worker_r.join();
	worker_s.join();
	close(my_sock);
}

void Client::send_msg(std::string msg){
	mtx.lock();
	strcat(res, msg.c_str());
	len = write(my_sock, res, strlen(res));
	if (len < 0) {
		std::cerr << "message not sent\n";
	}
	memset(res, '\0', sizeof(res));
	msg_send = msg_old;
	mtx.unlock();
}

std::string Client::get_user(){
	return username_str;
}
