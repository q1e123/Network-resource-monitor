#include "process.h"

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
