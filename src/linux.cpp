#ifdef __linux__

#include <fstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "linux.h"
#include "utils.h"
#include "jiffy.h"

using std::stoi;
using std::vector;
using std::ifstream, std::getline, std::cout;
using std::istringstream;
using std::pair;

string Linux::get_os(){
	vector<char> sep;
	sep.push_back(' ');
	sep.push_back('=');
	sep.push_back('"');
	return utils::get_value_sysfile(OS_RELEASE_FILE, "PRETTY_NAME",sep);
}

size_t Linux::get_total_ram(){
	vector<char> sep;
	sep.push_back(' ');
	sep.push_back(':');
	sep.push_back('\t');
	string tmp = utils::get_value_sysfile(MEMINFO_FILE, "MemTotal",sep);
	utils::remove_char_str(tmp, ' ');
	tmp = tmp.substr(0, tmp.size()-2);
	return stoi(tmp);
}

size_t Linux::get_avalabile_ram(){
	vector<char> sep;
	sep.push_back(' ');
	sep.push_back(':');
	sep.push_back('\t');
	string tmp = utils::get_value_sysfile(MEMINFO_FILE, "MemAvailable",sep);
	utils::remove_char_str(tmp,' ');
	tmp = tmp.substr(0, tmp.size()-2);
	return stoi(tmp);
}

map<string, Jiffy> get_jiffies(){
	map<string, Jiffy> jiffies;
	ifstream cpu_file(Linux::STAT_FILE);
	string line;
	while(getline(cpu_file,line)){
		istringstream linestream(line);
		string prop;
		linestream>>prop;
		if(prop.find("cpu") == string::npos){
			return jiffies;
		}
		vector<size_t> jfs;
		size_t jif;
		while(linestream>>jif){
			jfs.push_back(jif);
		}

		size_t total_jiffies = 0;
		for(auto jiffy:jfs){
			total_jiffies += jiffy;
		}
		size_t work_jiffies = 0;
		for(int i=0; i<3; ++i){
			work_jiffies += jfs[i];
		}
		Jiffy jiff(total_jiffies, work_jiffies);
		jiffies[prop] = jiff;
	}

	return jiffies;
}

map<string, double> Linux::get_cpu_usage(){
	map<string, Jiffy> current_jiffies, new_jiffies;
	current_jiffies= get_jiffies();
	usleep(100000);
	new_jiffies = get_jiffies();
	map<string, double> cpu_usage;
	for(auto item:current_jiffies){
		size_t dt, dw;
		dt = new_jiffies[item.first].get_total() - item.second.get_total(); 
		dw = new_jiffies[item.first].get_work() - item.second.get_work();
		double usage;
		usage = (dw * 1.0) / dt * 100;
		cpu_usage[item.first] = usage;
	}
	return cpu_usage;
}

void Linux::shutdown(){
	utils::execute("shutdown now");
}

void Linux::restart(){
	utils::execute("reboot");
}

string Linux::get_ip(){
	return "0.0.0.0";
}

#endif
