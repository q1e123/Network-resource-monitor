#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <thread>

#if defined _WIN32 || defined _WIN64
#include <winsock2.h>
#endif

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

#include "logger.h"
#include "system.h"
#include "network-usage.h"
#include "database-structs.h"

class Database_Manager{
public:
    Database_Manager();
    
    void init();

    int get_user_role(std::string user, std::string machine_id);
    void update_system_status(int system_id, int system_status);
    void update_user(DB_Users db_users);
    void update_system(DB_Systems db_systems);

    void insert_system(DB_Systems db_system);
    void insert_user(DB_Users db_user);
    void insert_usage_data(System *system);
    
    DB_Systems get_system_from(std::string user);

    std::vector<std::string> get_active_systems();
    std::vector<std::string> get_inactive_systems();
    std::vector<DB_Users> get_all_users();
    std::vector<DB_Systems> get_all_systems();


private:
    const std::string init_file_name = "../Init/database.ini";
    std::string type, user, password, users_table, systems_table, database_name, connection_string;
    std::string usage_data_table, cpu_usage_table, network_usage_table, user_list_table;
    std::string environment_variables_table;
    std::string insert_systems_str, insert_users_str;

    void get_login_data();
    void get_create_data();
    void get_insert_data();

    void create_tables();
    void create_users_table();
    void create_systems_table();
    void create_usage_data_table();
    void create_cpu_usage_table();
    void create_network_usage_table();
    void create_user_list_table();
    void create_environment_variables_table();

    void insert_data();
    void insert_systems();
    void insert_users();

    void insert_cpu_usage(std::vector<DB_Cpu_Usage> db_cpu_usage_list);
    void insert_network_usage(std::vector<DB_Network_Usage> db_network_usage_list);
    void insert_user_list(std::vector<DB_User_List> db_user_list);
    void insert_environment_variables(std::vector<DB_Environment_Variables> db_environment_variable_list);
    
    std::string get_query(std::string file);

    std::vector<DB_Systems> get_active_systems_list();
    System* build_system(DB_Systems systems);

    DB_Usage_Data get_usage_data(int system_id);
    std::vector<DB_Cpu_Usage> get_cpu_usage(int usage_id);
    std::vector<DB_Network_Usage> get_network_usage(int usage_id);
    std::vector<DB_User_List> get_user_list(int usage_id);
    std::vector<DB_Environment_Variables> get_environment_variables(int usage_id);
};

class Database_Exception: public std::exception{
public:
    virtual const char* what() const throw();
private:
};

#endif