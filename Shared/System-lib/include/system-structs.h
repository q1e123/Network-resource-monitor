#ifndef SYSTEM_STRUCTS_H
#define SYSTEM_STRUCTS_H

#include <ctime>
#include <string>

typedef struct System_User{
    std::string username;
    std::time_t last_login;
}System_User;


#endif