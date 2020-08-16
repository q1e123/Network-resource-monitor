#include "database-manager.h"

#include <iostream>
#include <fstream>
#include "SimpleIni.h"

Database_Manager::Database_Manager(){

}

void Database_Manager::init(){
	get_login_data();
    get_create_data();
    create_tables();    
}

void Database_Manager::get_login_data(){
    CSimpleIniA ini;
	if (ini.LoadFile("../Init/database.ini") < 0) {
		std::cerr << "Can't database init file" << std::endl;
		exit(1);
	}
    
    this->type = ini.GetValue("login", "type");
    this->user = ini.GetValue("login", "user");
    this->password = ini.GetValue("login", "password");
    this->database_name = ini.GetValue("login", "database_name");

    connection_string = "db=" + database_name + " user=" + this->user + " password=" + this->password;
}

void Database_Manager::get_create_data(){
    CSimpleIniA ini;
	if (ini.LoadFile("../Init/database.ini") < 0) {
		std::cerr << "Can't database init file" << std::endl;
		exit(1);
	}
    
    this->users_table = ini.GetValue("create", "users_table");
    this->systems_table = ini.GetValue("create", "systems_table");
}

void Database_Manager::create_tables(){
    if(this->systems_table == "on"){
        create_systems_table();
    }
    if(this->users_table == "on"){
        create_users_table();
    }
}

void Database_Manager::create_users_table(){
    connection.open(type, connection_string);
    
    std::string query = get_query("../SQL/create-users-table.sql");
    connection << query;

    connection.close();
}

void Database_Manager::create_systems_table(){
    connection.open(type, connection_string);
    
    std::string query = get_query("../SQL/create-systems-table.sql");
    connection << query;

    connection.close();
}

std::string Database_Manager::get_query(std::string file){
    std::string query = "";

    std::fstream query_file(file);
    std::string line;
    while (getline(query_file, line)){
        query += line + " ";
    }

    return query;    
}

int Database_Manager::get_user_role(std::string user){
    connection.open(type, connection_string);

    int role;
    soci::indicator ind;

    std::string query = get_query("../SQL/get-user-role.sql");
    connection << query, soci::into(role, ind), soci::use(user);

    connection.close();

    if(ind == soci::i_ok){
        return role;
    }
    return -1;
}

void Database_Manager::update_user_role(std::string user, int user_role){
    connection.open(type, connection_string);

    int role;

    std::string query = get_query("../SQL/uptate-user-role.sql");
    connection << query, soci::use(user, "user"), soci::use(user_role, "user_role");

    connection.close();
}


const char* Database_Exception::what() const throw(){
    return "Database error";
}

