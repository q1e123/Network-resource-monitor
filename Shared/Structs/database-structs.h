#ifndef DATABASE_STRUCTS_H
#define DATABASE_STRUCTS_H

#include <ctime>

#include <string>

typedef struct DB_Usage_Data{
    int id;
    int system_id;
    int total_ram;
    int used_ram;
    int free_ram;
    double avalabile_space;
    std::string current_user;
    std::string operating_system;
    std::tm timestamp;
    std::string program_list;
}DB_Usage_Data;

typedef struct Cpu_Usage{
    int id;
    std::string cpu_name;
    double usage;
    int usage_id;
}DB_Cpu_Usage;

typedef struct DB_Network_Usage{
    int id;
    std::string interface_name;
    double rx,tx;
    std::string ipv4;
    int usage_id;
}DB_Network_Usage;

typedef struct DB_Systems{
    int id;
    int status;
    std::string machine_id;
}DB_Systems;

typedef struct DB_Users{
    int id;
    std::string username;
    int user_role;
    int system_id;
}DB_Users;

typedef struct DB_User_List{
    int id;
    std::string username;
    std::tm last_login;
    int usage_id;
}DB_User_List;

typedef struct DB_Environment_Variables{
    int id;
    std::string variable;
    std::string variable_value;
    int usage_id;
}DB_Environment_Variables;
#endif