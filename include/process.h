#ifndef PROCESS_H
#define PROCESS_H

#include <cstdio>
#include <string>

#include "jiffy.h"

using std::string;

class Process{
public:
	Process(size_t pid = 0, string name = "", size_t ram = 0, double cpu_usage = 0.0);

	size_t get_pid();
	size_t get_ram();
	double get_cpu_usage();
	string get_name();

	void set_cpu_usage(double usage);
private:
	size_t pid, ram;
	double cpu_usage;
	string name;
};

#endif
