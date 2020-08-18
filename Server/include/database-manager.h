#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#if defined _WIN32 || defined _WIN64
#include <winsock2.h>
#endif

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

#include <string>

#include "logger.h"

enum User_Role{
    NEW = -1,
    NORMAL,
    ADMINISTRATOR
};

class Database_Manager{
public:
    Database_Manager();
    
    void init();

    int get_user_role(std::string user);
    void update_user_role(std::string user, int user_role);

    void insert_system(std::string machine_id);
    void insert_user(std::string user, std::string user_rank, std::string machine_id);
    void insert_usage_data(std::string user, double cpu_usage, double ram_usage, double network_usage_rx, double network_usage_tx);
    
    int get_system_id_from(std::string user);
private:
    std::string type, user, password, users_table, systems_table, database_name, connection_string;
    std::string data_usage_table;
    soci::session connection; 

    void get_login_data();
    void get_create_data();
    void create_tables();
    void create_users_table();
    void create_systems_table();
    void create_data_usage_table();
    std::string get_query(std::string file);
    
};

class Database_Exception: public std::exception{
public:
    virtual const char* what() const throw();
private:
};

#endif