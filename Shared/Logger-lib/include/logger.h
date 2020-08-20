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

    size_t get_first_line();
    size_t get_last_line();
    
private:
    std::string init_file;
    size_t first_line, last_line;
    std::ofstream log_file;
    Timestamp time;
};

#endif
