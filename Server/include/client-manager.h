#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <string>

#include <SimpleIni.h>

#include "logger.h"

class Client_Manager{
public:
    Client_Manager(Logger *logger);
    ~Client_Manager();
    bool is_new(std::string user);
    bool is_admin(std::string user);

    void add(std::string user);
private:
	CSimpleIniA ini_clients;
    Logger *logger;
};

#endif