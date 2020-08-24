#include "server.h"

#include <iostream>
#include <chrono>

#include "communication-protocol.h"
#include "database-utils.h"

#define WAIT_PERIOD std::chrono::milliseconds(500)


Server::Server(std::string name, size_t sock) {
	logger = new Logger("server-logs.txt");
	database_manager.init();
	int res = socket_init();
	if (res != 0) {
		logger->add_error("socket init error");
		exit(0);
	}

	this->name = name;
	disc_users = "d;";
	port_number = sock;
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
#ifdef __linux__
	server_addr.sin_addr.s_addr = inet_addr(LOOPBACK_ADDR);
#elif defined _WIN32 || defined _WIN64
	InetPton(AF_INET, (PCWSTR)LOOPBACK_ADDR, &server_addr.sin_addr.s_addr);
#endif
	client_addr_size = sizeof(client_addr);

	if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
		logger->add_error("binding uunsuccessful");
		exit(1);
	}

	if (listen(server_sock, 5) != 0) {
		logger->add_error("listening unsuccessful");
		exit(1);
	}
}

void Server::start() {
	while (1) {
		SOCKET client_sock;
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
		if (!socket_check(client_sock)) {
			logger->add_error("accept unsuccessful");
			socket_close(client_sock);
			socket_quit();
			break;
		}
		mtx.lock();
		inet_ntop(AF_INET, (struct sockaddr*)&client_addr, ip, INET_ADDRSTRLEN);
		logger->add_network("CONN", "successful", ip);

		Client_Info client(client_sock, ip);
		std::string user = Communication_Protocol::recv_message(client_sock, logger);
		std::string machine_id = Communication_Protocol::recv_message(client_sock, logger);
		client.set_user(user);

		Communication_Protocol::send_message(client_sock, this->name, logger);

		int user_role = database_manager.get_user_role(user, machine_id);
		std::string login_message;
		switch (user_role){
			case 0:
				login_message = "OK";
				break;
			case 1:
				login_message = "OK_ADMIN";
				break;			
			default:
				login_message = "RETRY";
				break;
		}
		Communication_Protocol::send_message(client_sock, login_message, logger);
		if(login_message == "OK" || login_message == "OK_ADMIN"){
			int system_id = database_manager.get_system_id_from(user);
			database_manager.update_system_status(system_id, 1);
			clients.push_back(client);
			std::thread worker(&Server::recv_msg, this, client);
			workers[client_sock] = std::move(worker);
		}
		
		mtx.unlock();
	}
	for (auto client : clients) {
		if (workers[client.get_socket_number()].joinable()) {
			workers[client.get_socket_number()].join();
		}
	}
}

void Server::send_to_all() {
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
			logger->add_error("sending error");
			continue;
		}else{
			logger->add_network("SEND", pkg, client.get_ip());
		}
	}
	mtx.unlock();
	std::this_thread::sleep_for(WAIT_PERIOD);
}

void Server::send_to(Client_Info client, std::string message) {
	mtx.lock();
	char* c_pkg = const_cast<char*>(message.c_str());
	if (send(client.get_socket_number(), c_pkg, strlen(c_pkg), 0) < 0) {
		logger->add_error("sending error");
	}else{
		logger->add_network("SEND", message, client.get_ip());
	}
	mtx.unlock();
}

void Server::recv_msg(Client_Info client) {
	logger->add("RECIVER STARTED FOR " + client.get_user());

	std::string package;
	try{
		while(package != "SOCKET_DOWN"){
				package = Communication_Protocol::recv_message(client.get_socket_number(), logger);
				run_cmd(package);
		}
	} catch (Client_Down_Exception e){
			int system_id = database_manager.get_system_id_from(client.get_user());
			database_manager.update_system_status(system_id, 0);
			logger->add_network("CONN", "disconnection", client.get_ip());
			mtx.lock(); 
			disc_users += client.get_user() + ";";
			remove_user(client.get_user());
			mtx.unlock();
	}
}

void Server::run_cmd(std::string cmd) {
	std::istringstream iss(cmd);
	std::string type;
	getline(iss, type, ';');

	mtx.lock();
	if (type == "SYS") {
		std::string user, serialization;
		getline(iss, user, ';');
		getline(iss, serialization);		
		cmd_sys(serialization);
	} else if (type == "LOG") {
		std::string user;
		getline(iss, user, ';');
		std::string log_size_str;
		getline(iss, log_size_str, ';');
		size_t log_size = std::stol(log_size_str);
		cmd_log(user, log_size);
	}else if (type == "REQ") {
		std::string request_type, user;
		getline(iss, request_type, ';');
		getline(iss, user, ';');
		if(request_type == "SYS_A"){
			cmd_req_sys_a(user);
		} else if (request_type == "SYS_I"){
			cmd_req_sys_i(user);
		} else if (request_type == "USERS"){
			cmd_req_users(user);
		}
		
	} else if (type == "UPDATE") {
		std::string update_type, user;
		getline(iss, update_type, ';');
		getline(iss, user, ';');
		if(update_type == "USERS"){
			cmd_update_users(user);
		}

	}
	mtx.unlock();
}

void Server::cmd_sys(std::string serialization) {
	System *sys = new System(serialization);
	if(!sys->sanity_check()){
		throw Sanity_Check_Failed_Exception();
	}
	database_manager.insert_usage_data(sys);
}

void Server::cmd_log(std::string user, size_t number_of_logs){
	size_t pos = find_client(user);
	Client_Info client = clients[pos];
	for (size_t i = 0; i < number_of_logs; i++){
		std::string log = Communication_Protocol::recv_message(client.get_socket_number(), logger);	
		size_t start = log.find("SYSTEM");
		std::string serialization = log.substr(start + 7);
		cmd_sys(serialization);
	}
}

void Server::cmd_req_sys_a(std::string user){
	size_t pos = find_client(user);
	Client_Info client = clients[pos];

	std::vector<std::string> systems = database_manager.get_active_systems();
	size_t number_of_systems = systems.size();
	std::string message = "SEND;SYS_A;" + std::to_string(number_of_systems);
	Communication_Protocol::send_message(client.get_socket_number() , message, logger);
	for(auto sys : systems){
		Communication_Protocol::send_message(client.get_socket_number(), sys, logger);
	}
}

void Server::cmd_req_sys_i(std::string user){
	size_t pos = find_client(user);
	Client_Info client = clients[pos];

	std::vector<std::string> systems = database_manager.get_inactive_systems();
	size_t number_of_systems = systems.size();
	Communication_Protocol::send_message(client.get_socket_number() , std::to_string(number_of_systems), logger);
	for(auto sys : systems){
		Communication_Protocol::send_message(client.get_socket_number(), sys, logger);
	}
}

void Server::cmd_req_users(std::string user){
	size_t pos = find_client(user);
	Client_Info client = clients[pos];

	std::vector<DB_Users> users = database_manager.get_all_users();
	size_t number_of_systems = users.size();
	std::string message = "SEND;USERS;" + std::to_string(number_of_systems);

	Communication_Protocol::send_message(client.get_socket_number() , message, logger);
	for(auto user : users){
		std::string serialization = Database_Structs_Utils::serialize(user);
		Communication_Protocol::send_message(client.get_socket_number(), serialization, logger);
	}
}
void Server::cmd_update_users(std::string user){
	size_t pos = find_client(user);
	Client_Info client = clients[pos];

	std::string number_of_users_str;
	getline(iss, number_of_users_str,';');
	number_of_users = std::stol(number_of_users_str);

	for (size_t i = 0; i < number_of_users; i++){
		std::string serialization = Communication_Protocol::recv_message(client.get_socket_number(), logger);
		DB_Users db_user = Database_Structs_Utils::deserialize_db_users(serialization);
		database_manager.update_user(db_user);
	}
}

void Server::remove_user(std::string user) {
	size_t pos = find_client(user);
	clients.erase(clients.begin() + pos);
	systems.erase(user);
}

size_t Server::find_client(std::string user){	
	for (size_t pos = 0; pos < clients.size(); ++pos) {
		if (clients[pos].get_user() == user) {
			return pos;
		}
	}
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

const char* Sanity_Check_Failed_Exception::what() const throw(){
    return "Sanity check failed";
}

