#include <iostream>
#include <chrono>

#define WAIT_PERIOD std::chrono::milliseconds(500)

#include "server.h"

Server::Server(std::string name, size_t sock) {

	int res = socket_init();
	if (res != 0) {
		std::cerr << "socket init error\m";
		exit(0);
	}

	this->name = name;
	disc_users = "d;";
	portno = sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(my_addr.sin_zero, '\0', sizeof(my_addr.sin_zero));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(portno);
#ifdef __linux__
	my_addr.sin_addr.s_addr = inet_addr(LOOPBACK_ADDR);
#elif defined _WIN32 || defined _WIN64
	InetPton(AF_INET, (PCWSTR)LOOPBACK_ADDR, &my_addr.sin_addr.s_addr);
#endif
	their_addr_size = sizeof(their_addr);

	if (bind(my_sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) != 0) {
		std::cerr << "binding uunsuccessful\n";
		exit(1);
	}

	if (listen(my_sock, 5) != 0) {
		std::cerr << "listening unsuccessful\n";
		exit(1);
	}
}

void Server::start() {
	send_worker = std::thread(&Server::send_to_all, this);
	while (1) {
		their_sock = accept(my_sock, (struct sockaddr*)&their_addr, &their_addr_size);
		if (!socket_check(their_sock)) {
			std::cerr << "accept unsuccessful";
			socket_close(their_sock);
			socket_quit();
			break;
		}
		mtx.lock();
		inet_ntop(AF_INET, (struct sockaddr*)&their_addr, ip, INET_ADDRSTRLEN);
		Client_Info client(their_sock, ip);
		const char* sv_name = this->name.c_str();
		if (send(client.get_socket_number(), sv_name, strlen(sv_name), 0) < 0) {
			std::cerr << "sending error\n";
			continue;
		}
		char* msg = (char*)"";
		if ((len = recv(client.get_socket_number(), msg, 500, 0)) > 0) {
			msg[len] = '\0';
			client.set_user(msg);
			memset(msg, '\0', sizeof(msg));
		}
		std::cout << ip << " connected\n";
		clients.push_back(client);
		std::thread worker(&Server::recv_msg, this, client);
		workers[their_sock] = std::move(worker);
		mtx.unlock();
	}
	for (auto client : clients) {
		if (workers[client.get_socket_number()].joinable()) {
			workers[client.get_socket_number()].join();
		}
	}
	if (send_worker.joinable()) {
		send_worker.join();
	}
}

void Server::send_to_all() {
	while (1) {
		mtx.lock();
		std::string pkg = "";
		pkg += "s;";
		for (auto item : systems) {
			pkg += item.first + ";" + item.second + "|";
		}
		pkg += "@" + disc_users;
		disc_users = "d;";
		char* c_pkg = const_cast<char*>(pkg.c_str());
		for (auto client : clients) {
			if (send(client.get_socket_number(), c_pkg, strlen(c_pkg), 0) < 0) {
				std::cerr << "sending error\n";
				continue;
			}
		}
		mtx.unlock();
		std::this_thread::sleep_for(WAIT_PERIOD);
	}
}
void Server::send_msg(char* msg, int curr) {
	mtx.lock();
	for (auto client : clients) {
		if (client.get_socket_number() != curr) {
			if (send(client.get_socket_number(), msg, strlen(msg), 0) < 0) {
				std::cerr << "sending error\n";
				continue;
			}
		}
	}
	mtx.unlock();
}

void Server::recv_msg(Client_Info client) {
	char msg[500], username[100];
	int len;
	int i;
	int j;

	len = recv(client.get_socket_number(), username, 500, 0);
	username[len] = '\0';

	while ((len = recv(client.get_socket_number(), msg, 500, 0)) > 0) {
		msg[len] = '\0';
		run_cmd(msg);
		memset(msg, '\0', sizeof(msg));
	}
	mtx.lock();
	std::cout << client.get_ip() << " dissconnected\n";
	disc_users += client.get_user() + ";";
	remove_user(client.get_user());
	mtx.unlock();
}

void Server::run_cmd(std::string cmd) {
	std::istringstream iss(cmd);
	std::string type;
	getline(iss, type, ';');
	mtx.lock();
	if (type == "s") {
		cmd_sys(std::move(iss));
	}
	mtx.unlock();
}

void Server::cmd_sys(std::istringstream iss) {
	std::string user, info, tmp;
	getline(iss, user, ';');
	getline(iss, info);
	systems[user] = info;
}

void Server::remove_user(std::string user) {
	size_t pos;
	for (pos = 0; pos < clients.size(); ++pos) {
		if (clients[pos].get_user() == user) {
			clients.erase(clients.begin() + pos);
			break;
		}
	}
	systems.erase(user);
}

int Server::socket_init() {
#ifdef __linux__
	return 0;
#elif defined _WIN32 || defined _WIN64
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#endif
}

int Server::socket_quit() {
#ifdef __linux__
	return 0;
#elif defined _WIN32 || defined _WIN64
	return WSACleanup();
#endif
}

bool Server::socket_check(SOCKET socket) {
#ifdef __linux__
	if (socket < 0) {
		return false;
	}
	return true;
#elif defined _WIN32 || defined _WIN64
	if (socket == INVALID_SOCKET) {
		return false;
	}
	return true;
#endif
}

int Server::socket_close(SOCKET socket) {
	int status = 0;
#ifdef __linux__
	status = shutdown(socket, SHUT_RDWR);
	if (status == 0) { 
		status = close(socket);
	}
#elif defined _WIN32 || defined _WIN64
	status = shutdown(socket, SD_BOTH);
	if (status == 0) { 
		status = closesocket(socket);
	}
#endif
	return status;
}