#include "logger.h"

Logger::Logger(std::string file_name){
    log_file.open(file_name, std::ios_base::app);
    std::string start_message = "LOG\tLogger has started";
    add(start_message);
}

Logger::~Logger(){
    std::string stopped_message = "LOG\tLogger has stopped";
    add(stopped_message);
    log_file.close();
}

void Logger::add(std::string message){
    time.update();
    log_file << time;
    log_file << "\t" << message << std::endl; 
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