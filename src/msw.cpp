#ifdef _WIN32 || defined _WIN64
#include <windows.h>
#include <thread>
#include <chrono>
#include "msw.h"
#include "utils.h"
#include "jiffy.h"
string Msw::get_os(){
	return utils::remove_char_str(utils::execute("ver"),'\n');
}

size_t Msw::get_total_ram(){
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx(&statex);
	return statex.ullTotalPhys;
}

size_t Msw::get_avalabile_ram(){
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx(&statex);
	return statex.ullAvailPhys;
}

size_t filetime_to_int(const FILETIME &ft){
    return (((size_t)(ft.dwHighDateTime)) << 32) | ((size_t)ft.dwLowDateTime);
}

map<string, double> Msw::get_cpu_usage(){
    map<string, double> cpu_usage;
    double usage;
    FILETIME idleTime, kernelTime, userTime;
    if(GetSystemTimes(&idleTime, &kernelTime, &userTime)){
        static size_t old_total=0;
        static size_t old_idle=0;

        size_t new_total, new_idle, dt, di;

        new_total = filetime_to_int(kernelTime)+filetime_to_int(userTime);
        new_idle = filetime_to_int(idleTime);

        dt = new_total-old_total;
        di  = new_idle-old_idle;

        old_total = new_total;
        old_idle  = new_idle;

        usage = 1.0;
        if(dt > 0){
            usage -= ((double)di)/dt;
        }
    }
    else{
        usage = -1.0;
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(500));
    cpu_usage["cpu"] = usage*100;
    return cpu_usage;
}

void Msw::shutdown(){
	utils::execute("shutdown -s -t 0");
}

void Msw::restart(){
	utils::execute("shutdown -r -t 0");
}

string Msw::get_ip(){
	return "0.0.0.0";
}

#endif
