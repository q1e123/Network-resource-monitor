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

    int get_user_role(std::string user, std::string machine_id);
    void update_user_role(std::string user, int user_role);

    void insert_system(std::string machine_id);
    void insert_user(std::string user, int user_role, std::string machine_id);
    void insert_usage_data(std::string user, double cpu_usage, double ram_usage, double network_usage_rx, double network_usage_tx);
    
    int get_system_id_from(std::string user);

private:
    std::string type, user, password, users_table, systems_table, database_name, connection_string;
    std::string data_usage_table;
    std::string insert_systems_str, insert_users_str; 
    soci::session connection; 

    void get_login_data();
    void get_create_data();
    void get_insert_data();
    void create_tables();
    void create_users_table();
    void create_systems_table();
    void create_data_usage_table();
    void insert_data();
    void insert_systems();
    void insert_users();
    std::string get_query(std::string file);
    
};

class Database_Exception: public std::exception{
public:
    virtual const char* what() const throw();
private:
};

#endif