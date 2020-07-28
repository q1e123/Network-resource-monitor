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

namespace Proc_Utils{
	bool compare_name(Process p1, Process p2);
	bool compare_pid(Process p1, Process p2);
	bool compare_cpu(Process p1, Process p2);
	bool compare_ram(Process p1, Process p2);
	bool compare_name_reverse(Process p1, Process p2);
	bool compare_pid_reverse(Process p1, Process p2);
	bool compare_cpu_reverse(Process p1, Process p2);
	bool compare_ram_reverse(Process p1, Process p2);
}

#endif
