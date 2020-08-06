#include "client-manager.h"

#include <filesystem>

Client_Manager::Client_Manager(Logger *logger){
    this->logger = logger;
    if (ini_clients.LoadFile("Clients/clients.ini") < 0) {
		logger->add_error("Can't open clients.ini");
		exit(1);
	}
}

bool Client_Manager::is_new(std::string user){
    CSimpleIniA::TNamesDepend keys;
	ini_clients.GetAllKeys(user.c_str(), keys);
    if(keys.empty()){
        return true;
    }
    return false;
}

bool Client_Manager::is_admin(std::string user){
    std::string value;
    value = ini_clients.GetValue(user.c_str(), "admin");
    if(value == "true"){
        return true;
    }
    return false;
}

void Client_Manager::add(std::string user){
    ini_clients.SetValue(user.c_str(), "admin", "false");
    std::filesystem::create_directory("Clients/" + user);
    logger->add(std::string("CREATED ") + user);
    ini_clients.SaveFile("Clients/clients.ini");

}

Client_Manager::~Client_Manager(){
    ini_clients.SaveFile("Clients/clients.ini");
    delete logger;
    logger = 0;
}