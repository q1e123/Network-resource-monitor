#include "my-process.h"
#include <algorithm>

Process::Process(size_t pid, string name,size_t ram, double cpu_usage){
	this->pid = pid;
	this->ram = ram;
	this->cpu_usage = cpu_usage;
	this->name = name;
}

size_t Process::get_pid(){
	return pid;
}

size_t Process::get_ram(){
	return ram;
}

double Process::get_cpu_usage(){
	return cpu_usage;
}

string Process::get_name(){
	return name;
}

void Process::set_cpu_usage(double usage){
	cpu_usage = usage;
}

bool Proc_Utils::compare_name(Process p1, Process p2){
	string str1, str2;
	str1 = p1.get_name();
	str2 = p2.get_name();
	std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
	std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	return str1 < str2;
}

bool Proc_Utils::compare_pid(Process p1, Process p2){
	return p1.get_pid() < p2.get_pid();
}

bool Proc_Utils::compare_cpu(Process p1, Process p2){
	return p1.get_cpu_usage() < p2.get_cpu_usage();
}

bool Proc_Utils::compare_ram(Process p1, Process p2){
	return p1.get_ram() < p2.get_ram();
}

bool Proc_Utils::compare_name_reverse(Process p1, Process p2){
	string str1, str2;
	str1 = p1.get_name();
	str2 = p2.get_name();
	std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
	std::transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	return str1 > str2;
}

bool Proc_Utils::compare_pid_reverse(Process p1, Process p2){
	return p1.get_pid() > p2.get_pid();
}

bool Proc_Utils::compare_cpu_reverse(Process p1, Process p2){
	return p1.get_cpu_usage() > p2.get_cpu_usage();
}

bool Proc_Utils::compare_ram_reverse(Process p1, Process p2){
	return p1.get_ram() > p2.get_ram();
}
