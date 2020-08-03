#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

#include "timestamp.h"

class Logger{
public:
    Logger(std::string file_name = "log_file.txt");
    ~Logger();
    void add(std::string message);
    void add_error(std::string message);
    void add_network(std::string type, std::string network_message, std::string recipe);
private:
    std::ofstream log_file;
    Timestamp time;
    void add_starting_message();
};

#endif