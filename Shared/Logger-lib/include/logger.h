#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

#include "timestamp.h"

class Logger{
public:
    Logger(std::string file_name = "log_file.txt");
    ~Logger();
    void init(std::string init_file);    
    void add(std::string message);
    void add_error(std::string error_message);
    void add_network(std::string type, std::string network_message, std::string recipe);
    void add_system(std::string serialization);
    size_t get_first_line();
    size_t get_last_line();
    std::string get_file_name();
    
private:
    std::string init_file, file_name;
    size_t first_line, last_line;
    std::fstream log_file;
    Timestamp t;
};

#endif
