#include "database-utils.h"

#include <sstream>
#include <iostream>
std::string Database_Structs_Utils::serialize(DB_Users user){
    std::string serialization;
    serialization = std::to_string(user.id) + ";" + user.username + ";" + std::to_string(user.user_rank) + 
                    ";" + user.machine_id + ";" + std::to_string(user.system_id);
    return serialization;
}

DB_Users Database_Structs_Utils::deserialize_db_users(std::string serialization){
    std::istringstream iss(serialization);
    
    std::string id_str;
    getline(iss, id_str, ';');
    int id = std::stoi(id_str);

    std::string username;
    getline(iss, username, ';');

    std::string user_rank_str;
    getline(iss, user_rank_str, ';');

    int user_rank = std::stoi(user_rank_str);

    std::string machine_id;
    getline(iss, machine_id, ';');

    std::string system_id_str;
    getline(iss, system_id_str, ';');

    int system_id = std::stoi(system_id_str);

    DB_Users db_user;
    db_user.id = id;
    db_user.username = username;
    db_user.user_rank = user_rank;
    db_user.machine_id = machine_id;
    db_user.system_id = system_id;

    return db_user;
}


std::string Database_Structs_Utils::serialize(DB_Systems db_system){
    std::string serialization;
    serialization = std::to_string(db_system.id) + ";" + std::to_string(db_system.status) + ";" +
                    db_system.machine_id;
    return serialization;
}

DB_Systems Database_Structs_Utils::deserialize_db_system(std::string serialization){
    std::istringstream iss(serialization);
    
    std::string id_str;
    getline(iss, id_str, ';');
    int id = std::stoi(id_str);

    std::string status_str;
    getline(iss, status_str, ';');

    int status = std::stoi(status_str);

    std::string machine_id;
    getline(iss, machine_id, ';');

    DB_Systems db_system;
    db_system.id = id;
    db_system.status = status;
    db_system.machine_id = machine_id;

    return db_system;
}