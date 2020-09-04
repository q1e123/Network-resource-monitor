#ifndef MY_PROCESS_H
#define MY_PROCESS_H

#include <cstdio>
#include <string>

#include "jiffy.h"
#include "build-opts-system.h"

class SYSTEM_IMPEXP Process{
public:
	Process(size_t pid = 0, std::string name = "", size_t ram = 0, double cpu_usage = 0.0);

	size_t get_pid();
	size_t get_ram();
	double get_cpu_usage();
	std::string get_name();

	void set_cpu_usage(double usage);
private:
	size_t pid, ram;
	double cpu_usage;
	std::string name;
};

namespace Proc_Utils{
	bool SYSTEM_IMPEXP compare_name(Process p1, Process p2);
	bool SYSTEM_IMPEXP compare_pid(Process p1, Process p2);
	bool SYSTEM_IMPEXP compare_cpu(Process p1, Process p2);
	bool SYSTEM_IMPEXP compare_ram(Process p1, Process p2);
	bool SYSTEM_IMPEXP compare_name_reverse(Process p1, Process p2);
	bool SYSTEM_IMPEXP compare_pid_reverse(Process p1, Process p2);
	bool SYSTEM_IMPEXP compare_cpu_reverse(Process p1, Process p2);
	bool SYSTEM_IMPEXP compare_ram_reverse(Process p1, Process p2);
}

#endif
