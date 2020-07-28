#include "client.h"
Client::Client(std::string user, std::string server_ip, size_t sock) {
	if (socket_init() != 0) {
		std::cout << "socket init failed\n";
	}
	username_str = user;
	username = const_cast<char*>(user.c_str());
	portno = sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0); 
	memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(portno);
	their_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

	if (connect(my_sock, (struct sockaddr*)&their_addr, sizeof(their_addr)) < 0) {
		std::cerr << "connection not established\n";
		exit(1);
	}

	std::cout << "connected";

	char msg[500];
	int len;
	if ((len = recv(my_sock, msg, 500, 0)) > 0) {
		msg[len] = '\0';
		mtx.lock();
		server_name = msg;
		msg_rec = msg;
		mtx.unlock();
		memset(msg, '\0', sizeof(msg));
	}
	len = send(my_sock, username, strlen(username), NULL);
	if (len < 0) {
		std::cerr << "initial identity message not sent\n";
		exit(1);
	}
	worker_r = std::thread(&Client::recv_msg, this);
	//worker_s = std::thread(&Client::send_msg, this);
}


void Client::recv_msg() {
	char msg[500];
	int len;
	while ((len = recv(my_sock, msg, 500, 0)) > 0) {
		msg[len] = '\0';
		mtx.lock();
		//	std::cout<<msg<<"\n";
		msg_rec = msg;
		mtx.unlock();
		memset(msg, '\0', sizeof(msg));
	}
}

Client::Client() {
}
Client::~Client() {
	worker_r.join();
	worker_s.join();
	socket_close(my_sock);
}

void Client::send_msg(std::string msg) {
	mtx.lock();
	STRCAT(res, msg.c_str());
	len = send(my_sock, res, strlen(res), NULL);
	if (len < 0) {
		std::cerr << "message not sent\n";
	}
	memset(res, '\0', sizeof(res));
	msg_send = msg_old;
	mtx.unlock();
}

std::string Client::get_user() {
	return username_str;
}

std::string Client::get_msg_rec() {
	return msg_rec;
}

std::string Client::get_server_name() {
	return server_name;
}

int Client::socket_init() {
#ifdef __linux__
	return 0;
#elif defined _WIN32 || defined _WIN64
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#endif
}

int Client::socket_quit() {
#ifdef __linux__
	return 0;
#elif defined _WIN32 || defined _WIN64
	return WSACleanup();
#endif
}

bool Client::socket_check(SOCKET socket) {
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

int Client::socket_close(SOCKET socket) {
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
