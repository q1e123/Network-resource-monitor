#include "timestamp.h"

Timestamp::Timestamp(){
    update();
}

void Timestamp::update(){
    now = time(0);
    local_time = localtime(&now);
    get_current_time();
}

void Timestamp::get_current_time(){
    year = 1900 + local_time->tm_year;
    month = 1 + local_time->tm_mon;
    day = local_time->tm_mday;
    hour = 1 + local_time->tm_hour;
    minutes = 1 + local_time->tm_min;
    seconds = 1 + local_time->tm_sec;
}

size_t Timestamp::get_year(){
    return year;
}

size_t Timestamp::get_month(){
    return month;
}

size_t Timestamp::get_day(){
    return day;
}

size_t Timestamp::get_hour(){
    return hour;
}

size_t Timestamp::get_minutes(){
    return minutes;
}

size_t Timestamp::get_seconds(){
    return seconds;
}
