#ifdef __linux__

#include <fstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <dirent.h>
#include <tuple>

#include "linux.h"
#include "utils.h"
#include "jiffy.h"

using std::stol;
using std::vector;
using std::ifstream, std::getline, std::cout;
using std::istringstream;
using std::pair;
using std::to_string;
using std::tuple;

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
	return stol(tmp);
}

size_t Linux::get_avalabile_ram(){
	vector<char> sep;
	sep.push_back(' ');
	sep.push_back(':');
	sep.push_back('\t');
	string tmp = utils::get_value_sysfile(MEMINFO_FILE, "MemAvailable",sep);
	utils::remove_char_str(tmp,' ');
	tmp = tmp.substr(0, tmp.size()-2);
	return stol(tmp);
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
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
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

vector<string> Linux::get_network_interfaces(){
	utils::execute("ls /sys/class/net/ > interfaces");

	ifstream interfaces_file("interfaces");
	vector<string> interfaces;
	string interface;
	while(interfaces_file>>interface){
		interfaces.push_back(interface);
	}

	return interfaces;
}

Network_Usage Linux::get_interface_usage(string interface){
	string start,cmd_rx, cmd_tx,stat, txp, rxp;
	start = "cat /sys/class/net/";
	stat = "/statistics/";
	txp = "tx_packets";
	rxp = "rx_packets";
	cmd_rx = start + interface + stat + rxp;
	cmd_tx = start + interface + stat + txp;
	
	size_t rx_old, tx_old;
	rx_old = std::stol(utils::execute(cmd_rx.c_str()));
	tx_old = std::stol(utils::execute(cmd_tx.c_str()));
	Network_Usage nu_old(rx_old, tx_old);
	std::this_thread::sleep_for (std::chrono::milliseconds(100));
	size_t rx_new, tx_new;
	rx_new = std::stol(utils::execute(cmd_rx.c_str()));
	tx_new = std::stol(utils::execute(cmd_tx.c_str()));
	Network_Usage nu_new(rx_new, tx_new);

	return nu_new - nu_old;
}

map<string, Network_Usage> Linux::get_network_usage(){
	map<string, Network_Usage> network_usage;
	vector<string> interfaces;
	interfaces = Linux::get_network_interfaces();
	
	string start,cmd_rx, cmd_tx,stat, txp, rxp;
	start = "cat /sys/class/net/";
	stat = "/statistics/";
	txp = "tx_packets";
	rxp = "rx_packets";

	vector<Network_Usage> old_usages;
	for(auto interface : interfaces){
		size_t rx_old, tx_old;
		cmd_rx = start + interface + stat + rxp;
		cmd_tx = start + interface + stat + txp;
		rx_old = std::stol(utils::execute(cmd_rx.c_str()));
		tx_old = std::stol(utils::execute(cmd_tx.c_str()));
		old_usages.push_back(Network_Usage(rx_old, tx_old));
	}
	std::this_thread::sleep_for (std::chrono::milliseconds(100));
	vector<Network_Usage> new_usages;
	for(auto interface : interfaces){
		size_t rx_new, tx_new;
		cmd_rx = start + interface + stat + rxp;
		cmd_tx = start + interface + stat + txp;
		rx_new = std::stol(utils::execute(cmd_rx.c_str()));
		tx_new = std::stol(utils::execute(cmd_tx.c_str()));
		new_usages.push_back(Network_Usage(rx_new, tx_new));
	}
	for(size_t i = 0; i < interfaces.size();++i){
		network_usage[interfaces[i]] = new_usages[i] - old_usages[i];
	}
	return network_usage;
}

vector<size_t> Linux::get_pids() {
  vector<size_t> pids;
  DIR* directory = opendir(Linux::PROC_DIR.c_str());
  struct dirent* file;
  while ((file = readdir(directory))) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        size_t pid = stol(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

size_t get_total_cpu(){
	ifstream cpu_file(Linux::STAT_FILE);
	string line;
	getline(cpu_file,line);
	istringstream linestream(line);
	string str;
	linestream>>str;
	size_t jif, total_jiffies;
	total_jiffies = 0;
	while(linestream>>jif){
		total_jiffies +=jif;
	}
	return total_jiffies;
}

void Linux::get_proc_info(size_t pid, Process &process){
	string filename, line, data, name;
	size_t line_number, jiffies_old, uptime_old;
	bool last;
	filename = Linux::PROC_DIR + to_string(pid) + "/stat";
	size_t total_cpu_old;
	total_cpu_old = get_total_cpu();
	ifstream proc_stat_file(filename);
	std::getline(proc_stat_file, line);	
	istringstream line_stream(line);
	jiffies_old = 0;
	line_number = 0;
	last = false;
	while(getline(line_stream, data, ' ') && !last){
		switch(line_number){
			case 1:{
				name = data;
				break;
			}
			case 13:{
				jiffies_old += stol(data);
				break;
			}
			case 14:{
				jiffies_old += stol(data);	
				break;
			}
			case 15:{
				jiffies_old += stol(data);
				break;
			}
			case 16:{
				jiffies_old += stol(data);	
				last = true;
				break;
			}
		}
		++line_number;
	}
	proc_stat_file.close();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));

	proc_stat_file.open(filename);
	size_t total_cpu_new;
	total_cpu_new = get_total_cpu();
	size_t jiffies_new,  ram;
	std::getline(proc_stat_file, line);	
	line_stream = istringstream(line);
	jiffies_new = 0;
	line_number = 0;
	last = false;
	while(getline(line_stream, data, ' ') && !last){
		switch(line_number){
			case 1:{
				name = data;
				break;
			}
			case 13:{
				jiffies_new += stol(data);
				break;
			}
			case 14:{
				jiffies_new += stol(data);	
				break;
			}
			case 15:{
				jiffies_new += stol(data);
				break;
			}
			case 16:{
				jiffies_new += stol(data);	
				break;
			}
			case 22:{
				ram = std::stol(data);
				last = true;
				break;
			}
		}
		++line_number;
	}
	double usage,du,dj;
	du = double(total_cpu_new - total_cpu_old);
	if(!du){
		usage = 0;
	}else{
		dj = double(jiffies_new - jiffies_old);
		usage = dj / du * 100;
	}
	
	process = Process(pid, name, ram, usage);
}

vector<Process> Linux::get_process_list(){
	vector<Process> proc_list;
	vector<size_t> pids;
	vector<std::thread> threads;
	pids = Linux::get_pids();
	proc_list = vector<Process>(pids.size(),Process());

	for(size_t i = 0; i <pids.size(); ++i){
		std::thread worker(Linux::get_proc_info, pids[i], std::ref(proc_list[i]));
		threads.push_back(std::move(worker));
	}

	for(auto &worker : threads){
		worker.join();	
	}
	return proc_list;
}

std::string Linux::get_machine_id(){
	std::string machine_id;
	machine_id = utils::execute("cat /etc/machine-id");
	machine_id += "-";
	machine_id += utils::execute("sudo cat /sys/class/dmi/id/product_uuid");
	machine_id = utils::remove_char_str(machine_id, '\n');
	return machine_id;
}

std::string Linux::get_current_user(){
	std::string user = utils::execute("whoami");
	user = utils::remove_char_str(user, '\n');
	return user;
}

std::vector<System_User> Linux::get_user_list(){
	std::vector<System_User> user_list;
	std::ifstream passwd_file("/etc/passwd");

	std::string line;
	while(getline(passwd_file, line)){
		std::istringstream iss(line);
		std::string user;
		getline(iss, user, ':');
		struct tm *t = get_last_login(user);
		std::time_t last_login = std::mktime(t);
		System_User sys_user;
		sys_user.username = user;
		sys_user.last_login = last_login;
		user_list.push_back(sys_user);
	}

	return user_list;
}

struct tm* Linux::get_last_login(std::string user){
	std::map<std::string, int> months;
	months["Jan"] = 0;
	months["Feb"] = 1;
	months["Mar"] = 2;
	months["Apr"] = 3;
	months["May"] = 4;
	months["June"] = 5;
	months["July"] = 6;
	months["Aug"] = 7;
	months["Sept"] = 8;
	months["Oct"] = 9;
	months["Nov"] = 10;
	months["Dec"] = 11;
	
	std::string check_str;
	std::string check_command = "lastlog -u " + user + " | tail -1 | awk '{print $3}'";
	std::string check_res = utils::execute(check_command.c_str());
	check_res = utils::remove_char_str(check_res, '\n');

	std::string month_command, day_command, time_command, year_command;
	struct tm *t = new tm;
	if(months.count(check_res)){
		month_command = "lastlog -u " + user + " | tail -1 | awk '{print $3}'";
		day_command = "lastlog -u " + user + " | tail -1 | awk '{print $4}'";
		time_command = "lastlog -u " + user + " | tail -1 | awk '{print $5}'";
		year_command = "lastlog -u " + user + " | tail -1 | awk '{print $7}'";
	}else{
		if(check_res == "logged"){
			return t;
		}
		month_command = "lastlog -u " + user + " | tail -1 | awk '{print $4}'";
		day_command = "lastlog -u " + user + " | tail -1 | awk '{print $5}'";
		time_command = "lastlog -u " + user + " | tail -1 | awk '{print $6}'";
		year_command = "lastlog -u " + user + " | tail -1 | awk '{print $8}'";
	}
	std::string month_res = utils::execute(month_command.c_str());
	month_res = utils::remove_char_str(month_res, '\n');
	std::string day_res = utils::execute(day_command.c_str());
	day_res = utils::remove_char_str(day_res, '\n');
	std::string time_res = utils::execute(time_command.c_str());
	time_res = utils::remove_char_str(time_res, '\n');
	std::string year_res = utils::execute(year_command.c_str());
	year_res = utils::remove_char_str(year_res, '\n');

	t->tm_mday = std::stoi(day_res);
	t->tm_mon = months[month_res];
	t->tm_year = std::stoi(year_res) - 1900;

	std::string hour_str, minutes_str, seconds_str;
	std::istringstream iss(time_res);
	getline(iss, hour_str, ':');
	getline(iss, minutes_str, ':');
	getline(iss, seconds_str);

	t->tm_hour = std::stoi(hour_str);
	t->tm_min = std::stoi(minutes_str);
	t->tm_sec = std::stoi(seconds_str);

	return t;
}

std::map<std::string, std::string> Linux::get_environment_variables(){
	std::map<std::string, std::string> environment_variables;
	std::string printev_result = utils::execute("printenv");
	std::istringstream printev_iss(printev_result);

	std::string key;
	while(getline(printev_iss, key, '=')){
		std::string value;
		getline(printev_iss, value, '\n');
		environment_variables[key] = value;
	}
	return environment_variables;
}

double Linux::get_avalabile_space(){
	unsigned long long total_avalabile_space_bytes = 0;
	std::string command_result = utils::execute("df | awk '{print $4}'");
	std::istringstream iss(command_result);
	std::string line;
	while (getline(iss, line, '\n')){
		try{
			unsigned long long filesystem_space = std::stoll(line);
			total_avalabile_space_bytes += filesystem_space;
		}catch(const std::exception& e){
			continue;
		}		
	}
	double total_avalabile_space = total_avalabile_space_bytes / 1024 / 1024;
	return total_avalabile_space;
}

std::vector<std::string> Linux::get_installed_programs(){
	std::vector<std::string> installed_programs;
	std::string ls_path_result = utils::execute("ls ${PATH//:/ }");
	std::istringstream ls_path_iss(ls_path_result);
	std::string program;
	while (getline(ls_path_iss, program, '\n')){
		if(program[0] == '/' || program.size() < 2){
			continue;
		}
		installed_programs.push_back(program);
	}
	return installed_programs;
	
}
#endif
