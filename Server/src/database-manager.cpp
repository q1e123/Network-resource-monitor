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
    this->user_list_table = ini.GetValue("create", "user_list_table");
    this->environment_variables_table = ini.GetValue("create", "environment_variables_table");
    this->program_list_table = ini.GetValue("create", "program_list_table");
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
    if(this->network_usage_table == "on"){
        create_network_usage_table();
    }
    if(this->user_list_table == "on"){
        create_user_list_table();
    }
    if(this->environment_variables_table == "on"){
        create_environment_variables_table();
    }
    if(this->program_list_table == "on"){
        create_program_list_table();
    }
}

void Database_Manager::create_users_table(){
    soci::session connection;
    connection.open(type, connection_string);
    std::string query = get_query("../SQL/create-users-table.sql");
    connection << query;
    connection.close();
}

void Database_Manager::create_systems_table(){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-systems-table.sql");
    connection << query;

    connection.close();
}

void Database_Manager::create_usage_data_table(){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-usage_data-table.sql");
    connection << query;

    connection.close();
}

void Database_Manager::create_cpu_usage_table(){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-cpu_usage-table.sql");
    connection << query;

    connection.close();

}

void Database_Manager::create_network_usage_table(){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-network_usage-table.sql");
    connection << query;

    connection.close();

}

void Database_Manager::create_user_list_table(){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-user_list-table.sql");
    connection << query;

    connection.close();
}

void Database_Manager::create_environment_variables_table(){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-environment_variables-table.sql");
    connection << query;

    connection.close();
}

void Database_Manager::create_program_list_table(){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/create-program_list-table.sql");
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
        DB_Systems sys;
        sys.machine_id = machine_id;
        insert_system(sys);
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

        std::string system_id_str = object + ".system_id";
        int system_id = std::stoi(ini.GetValue("users", system_id_str.c_str()));

        DB_Users db_users;
        db_users.username = user;
        db_users.user_role = user_role;
        db_users.system_id = system_id;

        insert_user(db_users);
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
    soci::session connection;
    connection.open(type, connection_string);

    int role;
    soci::indicator ind;

    std::string query = get_query("../SQL/get-user-role.sql");
    connection << query, soci::into(role, ind), soci::use(user, "user"),
                soci::use(machine_id, "machine_id");

    connection.close();

    if(ind == soci::i_ok){
        return role;
    }
    return -1;
}

void Database_Manager::update_system_status(int system_id, int system_status){
    soci::session connection;
    connection.open(type, connection_string);
    
    std::string query = get_query("../SQL/update-system_status.sql");
    connection << query, soci::use(system_status, "system_status"), soci::use(system_id, "id");

    connection.close();
}

void Database_Manager::insert_system(DB_Systems db_system){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/insert-system.sql");
    connection << query, soci::use(db_system.machine_id, "machine_id");

    connection.close();
}

void Database_Manager::insert_user(DB_Users db_user){
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/insert-user.sql");
    connection << query, soci::use(db_user.username, "user"), soci::use(db_user.user_role, "user_role"),
                soci::use(db_user.system_id, "system_id");

    connection.close();
}

void Database_Manager::insert_usage_data(System *system){
    DB_Systems db_sys = get_system_from(system->get_current_user());
    soci::session connection;

    connection.open(type, connection_string);
    
    int id;
    std::string query = get_query("../SQL/insert-usage_data.sql");
    std::time_t t = system->get_timestamp();
    std::tm *timestamp = std::localtime(&t);

    connection << query, soci::use(system->get_total_ram(), "total_ram"),
                soci::use(system->get_used_ram(), "used_ram"), soci::use(system->get_avalabile_ram(), "free_ram"),
                soci::use(*timestamp, "usage_timestamp"), soci::use(system->get_current_user(), "user"),
                soci::use(system->get_os(), "operating_system"), soci::use(db_sys.id, "system_id"),
                soci::use(system->get_avalabile_space(), "avalabile_space");


    query = get_query("../SQL/get-last-id-usage_data.sql");
    connection << query, soci::into(id);

    connection.close();

    std::vector<DB_Cpu_Usage> cpu_usage_list;
    for(auto item : system->get_cpu_usage()){
        DB_Cpu_Usage data;
        data.cpu_name = item.first;
        data.usage = item.second;
        data.usage_id = id;
        cpu_usage_list.push_back(data);
    }
    std::thread cpu_usage_worker(&Database_Manager::insert_cpu_usage, this, cpu_usage_list);

    std::vector<DB_Network_Usage> network_usage_list;
    std::map<std::string, std::string> ipv4_map = system->get_ipv4();
    for(auto item : system->get_network_usage()){
        DB_Network_Usage data;
        data.interface_name = item.first;
        data.rx = item.second.get_rx();
        data.tx = item.second.get_tx();
        data.usage_id = id;
        data.ipv4 = ipv4_map[data.interface_name];
        network_usage_list.push_back(data);
    }
    std::thread network_usage_worker(&Database_Manager::insert_network_usage, this, network_usage_list);

    std::vector<DB_User_List> user_list;
    for(auto user : system->get_user_list()){
        std::time_t t = user.last_login;
        std::tm *timestamp = std::localtime(&t);

        DB_User_List data;
        data.username = user.username;
        data.last_login = *timestamp;
        data.usage_id = id;
        
        user_list.push_back(data);
    }
    std::thread user_worker(&Database_Manager::insert_user_list, this, user_list);

    std::vector<DB_Environment_Variables> environment_variable_list;
    for(auto item : system->get_environment_variables()){
        DB_Environment_Variables data;
        data.usage_id = id;
        data.variable = item.first;
        data.variable_value = item.second;
        environment_variable_list.push_back(data);
    }
    std::thread environment_variable_worker(&Database_Manager::insert_environment_variables, this, environment_variable_list);
    
    std::vector<DB_Program_List> db_program_list;
    /*for(auto program : system->get_installed_programs()){
        DB_Program_List program_list;
        program_list.usage_id = id;
        program_list.software = program;
        db_program_list.push_back(program_list);
    }*/
    std::thread program_list_worker(&Database_Manager::insert_program_list, this, db_program_list);

    if(cpu_usage_worker.joinable() && environment_variable_worker.joinable() && 
        network_usage_worker.joinable() && program_list_worker.joinable() &&
        user_worker.joinable()){
            user_worker.join();
            program_list_worker.join();
            network_usage_worker.join();
            environment_variable_worker.join();
            cpu_usage_worker.join();
        }
}

void Database_Manager::insert_cpu_usage(std::vector<DB_Cpu_Usage> db_cpu_usage_list){
    soci::session connection;
    connection.open(type, connection_string);
    DB_Cpu_Usage db_cpu_usage;
    std::string query = get_query("../SQL/insert-cpu_usage.sql");
    soci::statement st = (connection.prepare << query, 
                                                soci::use(db_cpu_usage.cpu_name, "cpu_name"),
                                                soci::use(db_cpu_usage.usage, "usage"), 
                                                soci::use(db_cpu_usage.usage_id, "usage_id"));
    
    for(auto usage : db_cpu_usage_list){
        db_cpu_usage = usage;
        st.execute(true);
    }

    connection.close();
}

void Database_Manager::insert_network_usage(std::vector<DB_Network_Usage> db_network_usage_list){
    soci::session connection;
    connection.open(type, connection_string);

    DB_Network_Usage db_network_usage;

    std::string query = get_query("../SQL/insert-network_usage.sql");
    soci::statement st = (connection.prepare << query, 
                                                soci::use(db_network_usage.interface_name, "interface_name"), 
                                                soci::use(db_network_usage.rx, "rx"),
                                                soci::use(db_network_usage.tx, "tx"),
                                                soci::use(db_network_usage.ipv4, "ipv4"),
                                                soci::use(db_network_usage.usage_id, "usage_id"));
    for(auto usage : db_network_usage_list){
        db_network_usage = usage;
        st.execute(true);
    }

    connection.close();
}

void Database_Manager::insert_user_list(std::vector<DB_User_List> db_user_list){
    soci::session connection;
    connection.open(type, connection_string);
    DB_User_List data;
    std::string query = get_query("../SQL/insert-user_list.sql");
    soci::statement st = (connection.prepare << query, 
                                                soci::use(data.username, "username"),
                                                soci::use(data.last_login, "last_login"),
                                                soci::use(data.usage_id, "usage_id"));
    for(auto user : db_user_list){
        data = user;
        st.execute(true);
    }

    connection.close();
}

void Database_Manager::insert_environment_variables(std::vector<DB_Environment_Variables> environment_variable_list){
    soci::session connection;
    connection.open(type, connection_string);
    DB_Environment_Variables data;
    std::string query = get_query("../SQL/insert-environment_variables.sql");
    soci::statement st = (connection.prepare << query,  
                                                soci::use(data.variable,"variable"), 
                                                soci::use(data.variable_value, "variable_value"),
                                                soci::use(data.usage_id, "usage_id"));
    for(auto variable : environment_variable_list){
        data = variable;
        st.execute(true);
    }

    connection.close();
}

void Database_Manager::insert_program_list(std::vector<DB_Program_List> program_list){
    soci::session connection;
    connection.open(type, connection_string);
    DB_Program_List data;
    std::string query = get_query("../SQL/insert-program_list.sql");
    soci::statement st = (connection.prepare << query, soci::use(data.software,"software"), 
                                        soci::use(data.usage_id, "usage_id"));
    for(auto program : program_list){
        data = program;
        st.execute(true);
    }

    connection.close();
}

DB_Systems Database_Manager::get_system_from(std::string user){
    soci::session connection;
    connection.open(type, connection_string);

    DB_Systems db_sys;
    std::string query = get_query("../SQL/get-system-from-user.sql");
    connection << query, soci::into(db_sys.id),
                        soci::into(db_sys.status),
                        soci::into(db_sys.machine_id),
                        soci::use(user, "user");

    connection.close();
    
    return db_sys;
}

std::vector<DB_Systems> Database_Manager::get_active_systems_list(){
    std::vector<DB_Systems> systems;
    soci::session connection;
    
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
    std::vector<DB_User_List> user_list = get_user_list(usage_data.id);
    std::vector<DB_Environment_Variables> enviroment_variables = get_environment_variables(usage_data.id);
    std::vector<DB_Program_List> program_list = get_program_list(usage_data.id);

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
    serialization += ";" + std::to_string(std::mktime(&usage_data.timestamp)) + ";";
    for(auto db_user_list: user_list){
        time_t t = mktime(&db_user_list.last_login);
        serialization += db_user_list.username + ":" + std::to_string(t) + "|";
    }
    serialization.pop_back();
    serialization += ";" + std::to_string(usage_data.avalabile_space) + ";";
    for(auto program : program_list){
        serialization += program.software + ":";
    }
    serialization.pop_back();
    serialization += ";";

    for(auto item : network_usage){
        serialization += item.interface_name + ":" + item.ipv4 + "|";
	}
	serialization.pop_back();
	serialization += ";";

    for(auto env_var : enviroment_variables){
        serialization += env_var.variable + "\t" + env_var.variable_value + "#";
    }
    serialization.pop_back();
    System *system = new System(serialization);
    return system;
}

DB_Usage_Data Database_Manager::get_usage_data(int system_id){
    soci::session connection;
    connection.open(type, connection_string);

    DB_Usage_Data usage_data;
    usage_data.system_id = system_id;

    std::string query = get_query("../SQL/get-usage_data.sql");
    connection << query, soci::into(usage_data.id), soci::into(usage_data.total_ram), 
                soci::into(usage_data.used_ram), soci::into(usage_data.free_ram), 
                soci::into(usage_data.avalabile_space), soci::into(usage_data.timestamp),
                soci::into(usage_data.current_user), soci::into(usage_data.operating_system),
                soci::use(system_id, "system_id");
    connection.close();

    return usage_data;
}

std::vector<DB_Cpu_Usage> Database_Manager::get_cpu_usage(int usage_id){
    std::vector<DB_Cpu_Usage> usages;
    
    soci::session connection;
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
    
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-network_usage.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query, soci::use(usage_id, "usage_id")); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Network_Usage network_usage;
        network_usage.interface_name = r.get<std::string>(0);
        network_usage.rx = r.get<double>(1);
        network_usage.tx = r.get<double>(2);
        network_usage.ipv4 = r.get<std::string>(3);
        usages.push_back(network_usage);
    }
 
    connection.close();

    return usages;
}

std::vector<DB_User_List> Database_Manager::get_user_list(int usage_id){
    std::vector<DB_User_List> user_list;
    soci::session connection;
    
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-user_list.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query, soci::use(usage_id, "usage_id")); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_User_List user;
        user.username = r.get<std::string>(0);
        user.last_login = r.get<std::tm>(1);
        user_list.push_back(user);
    }
 
    connection.close();

    return user_list;
}

std::vector<DB_Environment_Variables> Database_Manager::get_environment_variables(int usage_id){
    std::vector<DB_Environment_Variables> environment_variables;
    soci::session connection;
    
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-environment_variables.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query, soci::use(usage_id, "usage_id")); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Environment_Variables env_var;
        env_var.variable = r.get<std::string>(0);
        env_var.variable_value = r.get<std::string>(1);
        environment_variables.push_back(env_var);
    }
 
    connection.close();

    return environment_variables;
}

std::vector<DB_Program_List> Database_Manager::get_program_list(int usage_id){
    std::vector<DB_Program_List> program_list;
    
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-environment_variables.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query, soci::use(usage_id, "usage_id")); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Program_List prg;
        prg.software = r.get<std::string>(0);
        program_list.push_back(prg);
    }
 
    connection.close();

    return program_list;
}

std::vector<std::string> Database_Manager::get_inactive_systems(){
    std::vector<std::string> machine_ids;
    
    soci::session connection;
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
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-all-users.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Users db_user;
        db_user.id = r.get<int>(0);
        db_user.username = r.get<std::string>(1);
        db_user.user_role = r.get<int>(2);
        db_user.system_id = r.get<int>(3);

        user_list.push_back(db_user);
    } 

    connection.close();

    return user_list;
}

void Database_Manager::update_user(DB_Users db_users){  
    soci::session connection;
    connection.open(type, connection_string);
    std::string query = get_query("../SQL/update-user.sql");
    connection << query, soci::use(db_users.id, "id"), soci::use(db_users.user_role, "user_role"),
                soci::use(db_users.system_id, "system_id");
    connection.close();
}

std::vector<DB_Systems> Database_Manager::get_all_systems(){
    std::vector<DB_Systems> system_list;
    soci::session connection;
    connection.open(type, connection_string);

    std::string query = get_query("../SQL/get-all-systems.sql");
    soci::rowset<soci::row> rs = (connection.prepare << query); 
 
    for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it) { 
        const soci::row& r = *it;
        DB_Systems db_systems;
        db_systems.id = r.get<int>(0);
        db_systems.status = r.get<int>(1);
        db_systems.machine_id = r.get<std::string>(2);

        system_list.push_back(db_systems);
    } 

    connection.close();

    return system_list;
}

void Database_Manager::update_system(DB_Systems db_systems){
    soci::session connection;
    connection.open(type, connection_string);
    std::string query = get_query("../SQL/update-system.sql");
    connection << query, soci::use(db_systems.id, "id"), soci::use(db_systems.machine_id, "machine_id");
    connection.close();
}

const char* Database_Exception::what() const throw(){
    return "Database error";
}

