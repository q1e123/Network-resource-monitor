#include <iostream>

#include "server.h"

Server::Server(std::string name, size_t sock){
	this->name = name;
	portno = sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(my_addr.sin_zero, '\0', sizeof(my_addr.sin_zero));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(portno);
	my_addr.sin_addr.s_addr = inet_addr(LOOPBACK_ADDR);
	their_addr_size = sizeof(their_addr);

	if (bind(my_sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0) {
		std::cerr << "binding uunsuccessful\n";
		exit(1);
	}

	if (listen(my_sock, 5) != 0) {
		std::cerr << "listening unsuccessful\n";
		exit(1);
	}
}

void Server::start(){
	while(1) {
		if ((their_sock = accept(my_sock, (struct sockaddr *)&their_addr, &their_addr_size)) < 0) {
			std::cerr << "accept unsuccessful";
			exit(1);
		}
		mtx.lock();
		inet_ntop(AF_INET, (struct sockaddr *)&their_addr, ip, INET_ADDRSTRLEN);
		std::cout << ip << " connected\n";
		Client_Info client(their_sock, ip);
		clients.push_back(client);
		const char *sv_name = this->name.c_str();
		if (send(client.get_socket_number(), sv_name, strlen(sv_name), 0) < 0) {
			std::cerr << "sending error\n";
			continue;
		}
		std::thread worker(&Server::recv_msg, this, client);
		worker.detach();
		workers[their_sock] = std::move(worker);
		mtx.unlock();
	}
}

void Server::send_msg(char *msg, int curr){
	int i;
	mtx.lock();
	for(auto client : clients){
		if (client.get_socket_number() != curr) {
			if (send(client.get_socket_number(), msg, strlen(msg), 0) < 0) {
				std::cerr << "sending error\n";
				continue;
			}
		}
	}
	mtx.unlock();
}

void Server::recv_msg(Client_Info client){
	char msg[500], username[100];
	int len;
	int i;
	int j;

	len = recv(client.get_socket_number(), username, 500, 0);
	username[len] = '\0';

	while ((len = recv(client.get_socket_number(), msg, 500, 0)) > 0) {
		msg[len] = '\0';
		std::cout << msg << "\n";
		send_msg(msg, client.get_socket_number());
		memset(msg,'\0', sizeof(msg));
	}
	mtx.lock();
	std::cout << client.get_ip() << " dissconnected\n";
	mtx.unlock();
	if(workers[client.get_socket_number()].joinable()){
		workers[client.get_socket_number()].join();
	}
}
