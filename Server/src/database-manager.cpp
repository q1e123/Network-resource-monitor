#include "database-manager.h"

#include <ctime>
#include <iostream>
#include <fstream>

#include "SimpleIni.h"

Database_Manager::Database_Manager(){

}

void Database_Manager::init(){
	get_login_data();
    get_create_data();
    create_tables();    
    get_insert_data();
    insert_data();
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
    this->data_usage_table = ini.GetValue("create", "data_usage_table");
}

void Database_Manager::get_insert_data(){
    CSimpleIniA ini;
	if (ini.LoadFile("../Init/database.ini") < 0) {
		std::cerr << "Can't database init file" << std::endl;
		exit(1);
	}
    
    this->insert_systems_str = ini.GetValue("insert", "systems");
    this->insert_users_str = ini.GetValue("insert", "users");
}

void Database_Manager::create_tables(){
    if(this->systems_table == "on"){
        create_systems_table();
    }
    if(this->users_table == "on"){
        create_users_table();
    }
    if(this->data_usage_table == "on"){
        create_data_usage_table();
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

void Database_Manager::create_data_usage_table(){
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-usage_data-table.sql");
    connection << query;

    connection.close();
}

void Database_Manager::insert_data(){
    if(this->insert_systems_str == "on"){
        insert_systems();
    }
    if(this->insert_users_str == "on"){
        insert_users();
    }
}
void Database_Manager::insert_systems(){
    CSimpleIniA ini;
	if (ini.LoadFile("../Init/database.ini") < 0) {
		std::cerr << "Can't database init file" << std::endl;
		exit(1);
	}
    
    size_t number_of_systems = std::stol(ini.GetValue("systems", "number_of_systems_to_be_created"));

    for (size_t i = 0; i < number_of_systems; ++i){
        std::string object = "system" + std::to_string(i);
        std::string key = object + ".machine_id";
        std::string machine_id = ini.GetValue("systems", key.c_str());
        insert_system(machine_id);
    }

}
void Database_Manager::insert_users(){
    CSimpleIniA ini;
	if (ini.LoadFile("../Init/database.ini") < 0) {
		std::cerr << "Can't database init file" << std::endl;
		exit(1);
	}
    
    size_t number_of_users = std::stol(ini.GetValue("users", "number_of_users_to_be_created"));
    for (size_t i = 0; i < number_of_users; ++i){
        std::string object = "user" + std::to_string(i);
        std::string user_key = object + ".user";
        std::string user = ini.GetValue("users", user_key.c_str());
        std::string user_role_key = object + ".user_role";

        int user_role = std::stoi(ini.GetValue("users", user_role_key.c_str()));

        std::string machine_id_key = object + ".machine_id";
        std::string machine_id = ini.GetValue("users", machine_id_key.c_str());

        insert_user(user, user_role, machine_id);
    }
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

int Database_Manager::get_user_role(std::string user, std::string machine_id){
    connection.open(type, connection_string);

    int role;
    soci::indicator ind;

    std::string query = get_query("../SQL/get-user-role.sql");
    connection << query, soci::into(role, ind), soci::use(user), soci::user(user);

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

void Database_Manager::insert_system(std::string machine_id){
    connection.open(type, connection_string);

    int role;

    std::string query = get_query("../SQL/insert-system.sql");
    connection << query, soci::use(machine_id, "machine_id");

    connection.close();
}

void Database_Manager::insert_user(std::string user, int user_role, std::string machine_id){
    connection.open(type, connection_string);

    int role;

    std::string query = get_query("../SQL/insert-user.sql");
    connection << query, soci::use(user, "user"), soci::use(user_role, "user_role"),soci::use(machine_id, "machine_id");

    connection.close();
}

void Database_Manager::insert_usage_data(std::string user, double cpu_usage, double ram_usage, double network_usage_rx, double network_usage_tx){
    std::time_t t = std::time(0);   // get time now
    std::tm *timestamp = std::localtime(&t);

    int system_id = get_system_id_from(user);

    connection.open(type, connection_string);

    int role;
    std::string query = get_query("../SQL/insert-usage-data.sql");
    connection << query, soci::use(cpu_usage, "cpu_usage"), soci::use(ram_usage, "ram_usage"),
                soci::use(network_usage_rx, "network_usage_rx"), soci::use(network_usage_tx, "network_usage_tx"),
                soci::use(*timestamp, "timestamp"), soci::use(system_id, "system_id");

    connection.close();
}

int Database_Manager::get_system_id_from(std::string user){
    connection.open(type, connection_string);

    int system_id;
    soci::indicator ind;

    std::string query = get_query("../SQL/get-system-id-from-user.sql");
    connection << query, soci::into(system_id, ind), soci::use(user);

    connection.close();

    if(ind == soci::i_ok){
        return system_id;
    }
    return -1;
}

const char* Database_Exception::what() const throw(){
    return "Database error";
}

