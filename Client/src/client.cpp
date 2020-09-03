#include "client.h"

#include <fstream>

#include "SimpleIni.h"

#include "communication-protocol.h"
#include "database-utils.h"

Client::Client(std::string user, std::string machine_id) {
	logger = new Logger("network-logs.txt");
	if (socket_init() != 0) {
		logger->add_error("socket init failed");
	}
	init();
	username = user;
	this->machine_id = machine_id;
	client_sock = socket(AF_INET, SOCK_STREAM, 0); 
	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
	server_addr.sin_addr.s_addr = inet_addr(ip_std.c_str());
	server_name = "NOT RECEIVED";
}

void Client::connect_to_server(){	
	if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		logger->add_error("connection not established");
		throw Server_Down_Exception();
	}

	logger->add_network("CONN", "successful", "server");
	Communication_Protocol::send_message(client_sock, username, logger);
	Communication_Protocol::send_message(client_sock, machine_id, logger);
	server_name = Communication_Protocol::recv_message(client_sock, logger);
	std::string login_response = Communication_Protocol::recv_message(client_sock, logger);
	if(login_response == "RETRY"){
		throw Login_Exception();
	}
	if(login_response == "OK"){
		this->role = "Normal user";
	}else if(login_response == "OK_ADMIN"){
		this->role = "Administrator";
	}else{
		this->role = "Contact dev";
	}
}

void Client::start_reciver(){
	reciver = std::thread(&Client::recive_message, this);
}

void Client::recive_message() {
	std::string package = "";
	while(package != "SOCKET_DOWN"){
		package = Communication_Protocol::recv_message(client_sock, logger);
		run_commannd(package);
	}
}

void Client::run_commannd(std::string command){
	std::istringstream iss(command);
	std::string type;
	getline(iss, type, ';');

	if (type == "SEND") {
		std::string send_type;
		getline(iss, send_type, ';');
		if(send_type == "SYS_A"){
			size_t number_of_systems;
			std::string number_of_systems_str;
			getline(iss, number_of_systems_str,';');
			number_of_systems = std::stol(number_of_systems_str);
			run_get_systems_active(number_of_systems);
		} else if (send_type == "SYS_I") {
			size_t number_of_systems;
			std::string number_of_systems_str;
			getline(iss, number_of_systems_str,';');
			number_of_systems = std::stol(number_of_systems_str);
			run_get_systems_inactive(number_of_systems);
		} else if (send_type == "USERS") {
			size_t number_of_users;
			std::string number_of_users_str;
			getline(iss, number_of_users_str,';');
			number_of_users = std::stol(number_of_users_str);
			run_get_users(number_of_users);
		} else if (send_type == "SYSTEMS") {
			size_t number_of_systems;
			std::string number_of_systems_str;
			getline(iss, number_of_systems_str,';');
			number_of_systems = std::stol(number_of_systems_str);
			run_get_systems(number_of_systems);
		} else if (send_type == "FILE") {
			std::string file_name;
			getline(iss, file_name);
			recv_file(file_name);
		}
	}else if (type == "REQ") {
		std::string request_type, user;
		getline(iss, request_type, ';');
		if(request_type == "FILE") {
			 std::string file_name;
			 getline(iss, file_name);
			 send_file(file_name);
		}
	} 	
}

void Client::run_get_systems_active(size_t number_of_systems){
	active_systems.clear();
	for (size_t i = 0; i < number_of_systems; i++){
		std::string serialization = Communication_Protocol::recv_message(this->client_sock, logger);
		System *sys = new System(serialization);
		active_systems.push_back(sys);
	}
}

void Client::run_get_systems_inactive(size_t number_of_systems){
	inactive_systems.clear();
	for (size_t i = 0; i < number_of_systems; i++){
		std::string machine_id = Communication_Protocol::recv_message(this->client_sock, logger);
		inactive_systems.push_back(machine_id);
	}
}

void Client::run_get_users(size_t number_of_users){
	users.clear();
	for (size_t i = 0; i < number_of_users; i++){
		std::string serialization = Communication_Protocol::recv_message(this->client_sock, logger);
		DB_Users db_user = Database_Structs_Utils::deserialize_db_users(serialization);
		users.push_back(db_user);
	}
}

void Client::run_get_systems(size_t number_of_systems){
	systems.clear();
	for (size_t i = 0; i < number_of_systems; ++i){
		std::string serialization = Communication_Protocol::recv_message(this->client_sock, logger);
		DB_Systems db_systems = Database_Structs_Utils::deserialize_db_system(serialization);
		systems.push_back(db_systems);
	}
}

std::vector<System*> Client::get_active_systems(){
	return this->active_systems;
}

std::vector<std::string> Client::get_inactive_systems(){
	return this->inactive_systems;
}

std::vector<DB_Users> Client::get_users(){
	return this->users;
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

std::string Client::get_role(){
	return role;
}

std::vector<DB_Systems> Client::get_systems(){
	return this->systems;
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

void Client::send_system_state(System *system){
	std::string message = "SYS;" + system->serilize();
	send_message(message);
}

void Client::send_log_file(Logger *logger){
	size_t number_of_logs = logger->get_last_line() - logger->get_first_line();
	std::string header = "LOG;" + std::to_string(number_of_logs);
	send_message(header);
	std::string log_file_name = logger->get_file_name();
	std::ifstream log_file(log_file_name);
	for (size_t i = 0; i < logger->get_first_line(); ++i){
		std::string dummy;
		getline(log_file, dummy);
	}

	std::string log;
	while (getline(log_file, log)){
		send_message(log);
	}
}

void Client::init(){
	CSimpleIniA ini;
	if (ini.LoadFile("../Init/client.ini") < 0) {
		std::cerr << "Can't open init file for client" << std::endl;
		exit(1);
	}

    this->ip_std = ini.GetValue("Network", "server_ip");
	this->port_number = std::stoi(ini.GetValue("Network",  "server_port"));
}

void Client::request_active_systems(){
	send_message("REQ;SYS_A;" + username);
}

void Client::request_inactive_systems(){
	send_message("REQ;SYS_I;" + username);
}

void Client::request_users(){
	send_message("REQ;USERS;" + username);
}

void Client::request_systems(){
	send_message("REQ;SYSTEMS;" + username);
}

void Client::update_users(std::vector<DB_Users> users){
	this->users = users;
	size_t number_of_systems = users.size();
	std::string message = "UPDATE;USERS;" + username +  ";" + std::to_string(number_of_systems);

	Communication_Protocol::send_message(this->client_sock, message, logger);
	for(auto user : users){
		std::string serialization = Database_Structs_Utils::serialize(user);
		Communication_Protocol::send_message(this->client_sock, serialization, logger);
	}
}

void Client::update_systems(std::vector<DB_Systems> systems){
	this->systems = systems;
	size_t number_of_systems = systems.size();
	std::string message = "UPDATE;SYSTEMS;" + username +  ";" + std::to_string(number_of_systems);

	Communication_Protocol::send_message(this->client_sock, message, logger);
	for(auto sys : systems){
		std::string serialization = Database_Structs_Utils::serialize(sys);
		Communication_Protocol::send_message(this->client_sock, serialization, logger);
	}
}

void Client::insert_user(DB_Users db_user){
	std::string header = "INSERT;USERS;" + username;
	send_message(header);
	std::string serialization = Database_Structs_Utils::serialize(db_user);
	send_message(serialization);
}

void Client::insert_system(DB_Systems db_system){
	std::string header = "INSERT;SYSTEMS;" + username;
	send_message(header);
	std::string serialization = Database_Structs_Utils::serialize(db_system);
	send_message(serialization);
}

void Client::send_file(std::string file_name){
	Communication_Protocol::send_file(this->client_sock, logger, file_name);
}

void Client::recv_file(std::string file_name){
	Communication_Protocol::recv_file(this->client_sock, logger, file_name);
}

const char* Server_Down_Exception::what() const throw(){
    return "Server down";
}

const char* Login_Exception::what() const throw(){
    return "Retry login";
}
