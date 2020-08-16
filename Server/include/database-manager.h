#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H
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
    
private:
    std::string type, user, password, users_table, systems_table, database_name, connection_string;
    soci::session connection; 

    void get_login_data();
    void get_create_data();
    void create_tables();
    void create_users_table();
    void create_systems_table();
    std::string get_query(std::string file);
    
};

class Database_Exception: public std::exception{
public:
    virtual const char* what() const throw();
private:
};

#endif