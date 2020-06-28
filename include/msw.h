#ifndef _WIN32 || defined _WIN64

#ifndef MSW_H
#define MSW_H
#include <fstream>
#include <string>

using std::string;

namespace Msw{
	string get_os();
	size_t get_total_ram();
	size_t get_avalabile_ram();
	size_t get_cpu_usage();
	void shutdown();
	void restart();
	string get_ip();
};

#endif
#endif
