#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <fstream>
#include <ctime>
#include <string>

class Timestamp{
public:
    Timestamp();

    void update();

    size_t get_year();
    size_t get_month();
    size_t get_day();
    size_t get_hour();
    size_t get_minutes();
    size_t get_seconds();
    
    //friend std::ofstream& operator<<(std::ofstream& os, const Timestamp& time);
    friend std::ofstream& operator<<(std::ofstream& fs, Timestamp &time){
        fs << time.get_year() << "-" << time.get_month() << "-" << time.get_day() <<
            " " << time.get_hour() << ":" << time.get_minutes() << ":" << time.get_seconds();
        return fs;
    }
private:
    size_t year, month, day, hour, minutes, seconds;
    time_t now;
    tm *local_time;

    void get_current_time();
};
#endif