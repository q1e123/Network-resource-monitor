#include "logger.h"

#include <iomanip>
#include <iostream>
#include "SimpleIni.h"

Logger::Logger(std::string file_name){
    this->file_name = file_name;
    this->first_line = this->last_line = 0;
    log_file.open(file_name, std::fstream::in | std::fstream::out | std::fstream::app);
    std::string start_message = "LOG\tLogger has started";
    add(start_message);
}
Logger::~Logger(){
    std::string stopped_message = "LOG\tLogger has stopped";
    add(stopped_message);
    log_file.close();
    CSimpleIniA ini;
    if(this->last_line != 0){
        if (ini.LoadFile(init_file.c_str()) < 0) {
            std::cerr << "Can't open init file for logger" << std::endl;
            exit(1);
        }
        ini.SetValue("Properties", "first_line", std::to_string(this->last_line).c_str());
        ini.SetValue("Properties", "last_line", std::to_string(this->last_line).c_str());
        ini.SaveFile(init_file.c_str());
    }
}
void Logger::add(std::string message){
    auto t = std::time(nullptr);
    auto local_time = *std::localtime(&t);
    log_file << std::put_time(&local_time, "%d-%m-%Y %H-%M-%S");
    log_file << "\t" << message << std::endl;
    ++(this->last_line);
}
void Logger::add_error(std::string error_message){
    std::string message = "ERR\t" + error_message;
    add(message);
}
void Logger::add_network(std::string type, std::string network_message, std::string recipe){
    if(type == "RECV"){
        recipe = "FROM " + recipe;
    } else if (type == "SEND"){
        recipe = "TO " + recipe;
    }
    std::string message = "NET\t" + type + " " + network_message + " " + recipe; 
    add(message);
}

void Logger::init(std::string init_file){
    this->init_file = init_file;
	CSimpleIniA ini;
	if (ini.LoadFile(init_file.c_str()) < 0) {
		std::cerr << "Can't open init file for logger" << std::endl;
		exit(1);
	}

    this->first_line = std::stol(ini.GetValue("Properties", "first_line"));
    this->last_line = std::stol(ini.GetValue("Properties", "last_line"));
}

size_t Logger::get_first_line(){
    return this->first_line;
}

size_t Logger::get_last_line(){
    return this->last_line;
}

std::string Logger::get_file_name(){
    return this->file_name;
}

void Logger::add_system(std::string serialization){
    std::string log = "SYSTEM " + serialization;
    add(log);
}