#include <iostream>
#include <string>
#include "server.h"
#include "database-manager.h"
int main(int argc, char *argv[]){
	if (argc != 3) {
		std::cerr << "Usage: ./server <server_name> <port>";
		exit(1);
	}
	
	Server server(argv[1], std::stol(argv[2]));
	server.start();
	return 0;
}
