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

std::vector<std::string> Linux::get_user_list(){
	std::vector<std::string> user_list;
	std::ifstream passwd_file("/etc/passwd");

	std::string line;
	while(getline(passwd_file, line)){
		std::istringstream iss(line);
		std::string user;
		getline(iss, user, ':');
		user_list.push_back(user);
	}
	return user_list;
}

#endif
