#include "client-info.h"

Client_Info::Client_Info(size_t socket_number, char ip[INET_ADDRSTRLEN]){
	this->socket_number = socket_number;
	this->ip = ip;
}

size_t Client_Info::get_socket_number(){
	return socket_number;
}

char* Client_Info::get_ip(){
	char *tmp = ip;
	return tmp;
}

std::string Client_Info::get_user(){
	return user;
}

void Client_Info::set_user(std::string user){
	this->user = user;
}
