#include "client.h"
Client::Client(std::string user, std::string server_ip, size_t sock) {
	if (socket_init() != 0) {
		std::cout << "socket init failed\n";
	}
	username_str = user;
	username = const_cast<char*>(user.c_str());
	port_number = sock;
	client_sock = socket(AF_INET, SOCK_STREAM, 0); 
	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
	server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

	if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "connection not established\n";
		exit(1);
	}

	std::cout << "connected";

	char msg[MESSAGE_SIZE];
	int len;
	if ((len = recv(client_sock, msg, MESSAGE_SIZE, 0)) > 0) {
		msg[len] = '\0';
		mtx.lock();
		server_name = msg;
		message_recived = msg;
		mtx.unlock();
		memset(msg, '\0', sizeof(msg));
	}
	len = send(client_sock, username, strlen(username), NULL);
	if (len < 0) {
		std::cerr << "initial identity message not sent\n";
		exit(1);
	}
	reciver = std::thread(&Client::recive_message, this);
}


void Client::recive_message() {
	char message[MESSAGE_SIZE];
	int len;
	while ((len = recv(client_sock, message, MESSAGE_SIZE, 0)) > 0) {
		message[len] = '\0';
		mtx.lock();
		message_recived = message;
		mtx.unlock();
		memset(message, '\0', sizeof(message));
	}
}

Client::Client() {
}
Client::~Client() {
	reciver.join();
	socket_close(client_sock);
}

void Client::send_msg(std::string msg) {
	mtx.lock();
	char package[MESSAGE_SIZE];

	STRCAT(package, msg.c_str());
	int len = send(client_sock, package, strlen(package), NULL);
	if (len < 0) {
		std::cerr << "message not sent\n";
	}
	mtx.unlock();
}

std::string Client::get_user() {
	return username_str;
}

std::string Client::get_message_recived() {
	return message_recived;
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
