#include "client.h"

#include "communication-protocol.h"

Client::Client(std::string user, std::string server_ip, size_t sock) {
	logger = new Logger("network-logs.txt");
	if (socket_init() != 0) {
		logger->add_error("socket init failed");
	}
	username = user;
	port_number = sock;
	client_sock = socket(AF_INET, SOCK_STREAM, 0); 
	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
	server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	server_name = "NOT RECEIVED";
}

void Client::connect_to_server(){
	if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		logger->add_error("connection not established");
	}

	logger->add_network("CONN", "successful", "server");
	server_name = Communication_Protocol::recv_message(client_sock, logger);
	Communication_Protocol::send_message(client_sock, username, logger);
}

void Client::start_reciver(){
	reciver = std::thread(&Client::recive_message, this);
}

void Client::recive_message() {
	std::string package = "";
	while(package != "SOCKET_DOWN"){
		package = Communication_Protocol::recv_message(client_sock, logger);
	}
}

Client::Client() {
}
Client::~Client() {
	delete logger;
	if(reciver.joinable()){
		reciver.join();
	}
	socket_close(client_sock);
}

void Client::send_message(std::string message) {
	Communication_Protocol::send_message(client_sock, message, logger);
}

std::string Client::get_user() {
	return std::string(username);
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
