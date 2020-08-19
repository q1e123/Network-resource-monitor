#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#if defined _WIN32 || defined _WIN64
#include <winsock2.h>
#endif

#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>

#include <string>

#include "logger.h"
#include "system.h"
#include "network-usage.h"

class Database_Manager{
public:
    Database_Manager();
    
    void init();

    int get_user_role(std::string user, std::string machine_id);
    void update_user_role(std::string user, int user_role);

    void insert_system(std::string machine_id);
    void insert_user(std::string user, int user_role, std::string machine_id);
    void insert_usage_data(System *system);
    
    int get_system_id_from(std::string user);

private:
    std::string type, user, password, users_table, systems_table, database_name, connection_string;
    std::string usage_data_table, cpu_usage_table, network_usage_table;
    std::string insert_systems_str, insert_users_str; 
    soci::session connection; 

    void get_login_data();
    void get_create_data();
    void get_insert_data();
    void create_tables();
    void create_users_table();
    void create_systems_table();
    void create_usage_data_table();
    void create_cpu_usage_table();
    void create_network_usage_table();
    void insert_data();
    void insert_systems();
    void insert_users();

    void insert_cpu_usage(std::string cpu_name, double usage, int usage_id);
    void insert_network_usage(std::string network_interface, Network_Usage usage, int usage_id);

    std::string get_query(std::string file);
    
};

class Database_Exception: public std::exception{
public:
    virtual const char* what() const throw();
private:
};

#endif