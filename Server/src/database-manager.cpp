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
	if (ini.LoadFile(this->init_file_name.c_str()) < 0) {
		std::cerr << "Can't open database init file" << std::endl;
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
	if (ini.LoadFile(this->init_file_name.c_str()) < 0) {
		std::cerr << "Can't open database init file" << std::endl;
		exit(1);
	}
    
    this->users_table = ini.GetValue("create", "users_table");
    this->systems_table = ini.GetValue("create", "systems_table");
    this->usage_data_table = ini.GetValue("create", "usage_data_table");
    this->cpu_usage_table = ini.GetValue("create", "cpu_usage_table");
    this->network_usage_table = ini.GetValue("create", "network_usage_table");
}

void Database_Manager::get_insert_data(){
    CSimpleIniA ini;
	if (ini.LoadFile(this->init_file_name.c_str()) < 0) {
		std::cerr << "Can't open database init file" << std::endl;
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
    if(this->usage_data_table == "on"){
        create_usage_data_table();
    }
    if(this->cpu_usage_table == "on"){
        create_cpu_usage_table();
    }
    if(this->network_usage_table== "on"){
        create_network_usage_table();
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

void Database_Manager::create_usage_data_table(){
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-usage_data-table.sql");
    connection << query;

    connection.close();
}

void Database_Manager::create_cpu_usage_table(){
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-cpu_usage-table.sql");
    connection << query;

    connection.close();

}

void Database_Manager::create_network_usage_table(){
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-network_usage-table.sql");
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
	if (ini.LoadFile(this->init_file_name.c_str()) < 0) {
		std::cerr << "Can't open database init file" << std::endl;
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
	if (ini.LoadFile(this->init_file_name.c_str()) < 0) {
		std::cerr << "Can't open database init file" << std::endl;
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

        std::string system_id_str = object + ".system_id";
        int system_id = std::stoi(ini.GetValue("users", system_id_str.c_str()));

        insert_user(user, user_role, machine_id, system_id);
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
    connection << query, soci::into(role, ind), soci::use(user, "user"), soci::use(machine_id, "machine_id");

    connection.close();

    if(ind == soci::i_ok){
        return role;
    }
    return -1;
}

void Database_Manager::update_user_role(std::string user, int user_role){
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/uptate-user-role.sql");
    connection << query, soci::use(user, "user"), soci::use(user_role, "user_role");

    connection.close();
}


void Database_Manager::update_system_status(int system_id, int system_status){
    connection.open(type, connection_string);
    
    std::string query = get_query("../SQL/update-system_status.sql");
    connection << query, soci::use(system_status, "system_status"), soci::use(system_id, "id");

    connection.close();
}

void Database_Manager::insert_system(std::string machine_id){
    connection.open(type, connection_string);

    int role;

    std::string query = get_query("../SQL/insert-system.sql");
    connection << query, soci::use(machine_id, "machine_id");

    connection.close();
}

void Database_Manager::insert_user(std::string user, int user_role, std::string machine_id, int system_id){
    connection.open(type, connection_string);

    int role;

    std::string query = get_query("../SQL/insert-user.sql");
    connection << query, soci::use(user, "user"), soci::use(user_role, "user_role"),soci::use(machine_id, "machine_id"),
                soci::use(system_id, "system_id");

    connection.close();
}

void Database_Manager::insert_usage_data(System *system){
    int system_id = get_system_id_from(system->get_current_user());

    connection.open(type, connection_string);
    
    int id;
    std::string query = get_query("../SQL/insert-usage_data.sql");
    std::time_t t = system->get_timestamp();
    std::tm *timestamp = std::localtime(&t);

    connection << query, soci::use(system->get_total_ram(), "total_ram"),
                soci::use(system->get_used_ram(), "used_ram"), soci::use(system->get_avalabile_ram(), "free_ram"),
                soci::use(*timestamp, "usage_timestamp"), soci::use(system->get_current_user(), "user"),
                soci::use(system->get_os(), "operating_system"), soci::use(system_id, "system_id");


    query = get_query("../SQL/get-last-id-usage_data.sql");
    connection << query, soci::into(id);

    connection.close();

    for(auto item : system->get_cpu_usage()){
        insert_cpu_usage(item.first, item.second, id);
    }

    for(auto item : system->get_network_usage()){
        insert_network_usage(item.first, item.second, id);
    }
}

void Database_Manager::insert_cpu_usage(std::string cpu_name, double usage, int usage_id){
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/insert-cpu_usage.sql");
    
    connection << query, soci::use(cpu_name, "cpu_name"), soci::use(usage, "usage"), soci::use(usage_id, "usage_id");

    connection.close();
}

void Database_Manager::insert_network_usage(std::string network_interface, Network_Usage usage, int usage_id){
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/insert-network_usage.sql");
    connection << query, soci::use(network_interface, "interface_name"), soci::use(usage.get_rx(), "rx"),
                 soci::use(usage.get_tx(), "tx") , soci::use(usage_id, "usage_id");

    connection.close();
}

int Database_Manager::get_system_id_from(std::string user){
    connection.open(type, connection_string);

    int system_id;
    soci::indicator ind;

    std::string query = get_query("../SQL/get-system-id-from-user.sql");
    connection << query, soci::into(system_id, ind), soci::use(user, "user");

    connection.close();

    if(ind == soci::i_ok){
        return system_id;
    }
    return -1;
}

std::vector<DB_Systems> Database_Manager::get_active_systems_list(){
    std::vector<DB_Systems> systems;
    
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-active-systems.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query); 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Systems sys;
        sys.id = r.get<int>(0);
        sys.status = r.get<int>(1);
        sys.machine_id = r.get<std::string>(2);
        systems.push_back(sys);
    } 

    connection.close();

    return systems;
}

System* Database_Manager::build_system(DB_Systems systems){
    DB_Usage_Data usage_data = get_usage_data(systems.id);
    std::vector<DB_Cpu_Usage> cpu_usage = get_cpu_usage(usage_data.id);
    std::vector<DB_Network_Usage> network_usage = get_network_usage(usage_data.id);
    std::string serialization;
    serialization = usage_data.operating_system;
    serialization += ";" + std::to_string(usage_data.total_ram);
    serialization += ";" + std::to_string(usage_data.free_ram);
    serialization += ";" + std::to_string(usage_data.used_ram);
    serialization += ";";
    for(auto usage : cpu_usage){
        std::string cpu_usage_str = std::to_string(usage.usage);
        serialization += usage.cpu_name + ":" + cpu_usage_str.substr(0, cpu_usage_str.size() - 4) + "-"; 
    }
    serialization.pop_back();
    serialization += ";";
    for(auto network_interface : network_usage){
        std::string rx = std::to_string(network_interface.rx);
        std::string tx = std::to_string(network_interface.tx);

        serialization += network_interface.interface_name + ":" + rx.substr(0, rx.size() - 4) + ":" +tx.substr(0, tx.size() - 4) + "|"; 
    }
    serialization.pop_back();
    serialization += ";" + usage_data.current_user;
    serialization += ";" + std::to_string(std::mktime(&usage_data.timestamp));
    System *system = new System(serialization);
    return system;
}

DB_Usage_Data Database_Manager::get_usage_data(int system_id){
    connection.open(type, connection_string);

    DB_Usage_Data usage_data;
    usage_data.system_id = system_id;

    std::string query = get_query("../SQL/get-usage_data.sql");
    connection << query, soci::into(usage_data.id), soci::into(usage_data.total_ram), 
                soci::into(usage_data.used_ram), soci::into(usage_data.free_ram), 
                soci::into(usage_data.timestamp), soci::into(usage_data.current_user), 
                soci::into(usage_data.operating_system) , soci::use(system_id, "system_id");
    connection.close();

    return usage_data;
}

std::vector<DB_Cpu_Usage> Database_Manager::get_cpu_usage(int usage_id){
    std::vector<DB_Cpu_Usage> usages;
    
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-cpu_usage.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query, soci::use(usage_id, "usage_id")); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Cpu_Usage cpu_usage;
        cpu_usage.cpu_name = r.get<std::string>(0);
        cpu_usage.usage = r.get<double>(1);
        usages.push_back(cpu_usage);
    }
 
    connection.close();

    return usages;
}

std::vector<DB_Network_Usage> Database_Manager::get_network_usage(int usage_id){
    std::vector<DB_Network_Usage> usages;
    
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-network_usage.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query, soci::use(usage_id, "usage_id")); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Network_Usage network_usage;
        network_usage.interface_name = r.get<std::string>(0);
        network_usage.rx = r.get<double>(1);
        network_usage.tx = r.get<double>(2);
        usages.push_back(network_usage);
    }
 
    connection.close();

    return usages;
}

std::vector<std::string> Database_Manager::get_inactive_systems(){
    std::vector<std::string> machine_ids;
    
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-inactive-systems.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        std::string machine_id = r.get<std::string>(0);
        machine_ids.push_back(machine_id);
    } 

    connection.close();

    return machine_ids;
}

std::vector<std::string> Database_Manager::get_active_systems(){
    std::vector<std::string> active_systems;
    std::vector<DB_Systems> db_system_list = get_active_systems_list();
 
    for(auto db_system : db_system_list){
        System *sys = build_system(db_system);
        std::string serialization = sys->serilize();
        active_systems.push_back(serialization);
    }
     
    return active_systems;
}

std::vector<DB_Users> Database_Manager::get_all_users(){
    std::vector<DB_Users> user_list;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-all-users.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Users db_user;
        db_user.id = r.get<int>(0);
        db_user.username = r.get<std::string>(1);
        db_user.user_rank = r.get<int>(2);
        db_user.machine_id = r.get<std::string>(3);
        db_user.system_id = r.get<int>(4);

        user_list.push_back(db_user);
    } 

    connection.close();

    return user_list;
}

void Database_Manager::update_user(DB_Users db_users){  
    connection.open(type, connection_string);
    std::string query = get_query("../SQL/update-user.sql");
    connection << query, soci::use(db_users.id, "id"), soci::use(db_users.user_rank, "user_role"),
                soci::use(db_users.machine_id, "machine_id"), soci::use(db_users.system_id, "system_id");
    connection.close();
}

const char* Database_Exception::what() const throw(){
    return "Database error";
}

