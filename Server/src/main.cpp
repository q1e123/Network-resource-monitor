#include <iostream>
#include <string>
#include "server.h"

int main(int argc, char *argv[]){
	if (argc != 2) {
		std::cerr << "Usage: ./server <port>";
		exit(1);
	}

	Server server(std::stol(argv[1]));
	server.start();

	return 0;
}
