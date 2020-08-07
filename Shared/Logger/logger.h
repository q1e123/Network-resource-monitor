#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

#include "timestamp.h"

class Logger{
public:
    Logger(std::string file_name = "log_file.txt"){
        log_file.open(file_name, std::ios_base::app);
        std::string start_message = "LOG\tLogger has started";
        add(start_message);
    }
    ~Logger(){
        std::string stopped_message = "LOG\tLogger has stopped";
        add(stopped_message);
        log_file.close();
    }
    void add(std::string message){
        time.update();
        log_file << time;
        log_file << "\t" << message << std::endl; 
    }
    void add_error(std::string error_message){
        std::string message = "ERR\t" + error_message;
        add(message);
    }
    void add_network(std::string type, std::string network_message, std::string recipe){
        if(type == "RECV"){
            recipe = "FROM " + recipe;
        } else if (type == "SEND"){
            recipe = "TO " + recipe;
        }
        std::string message = "NET\t" + type + " " + network_message + " " + recipe; 
        add(message);
    }
private:
    std::ofstream log_file;
    Timestamp time;
};

#endif
