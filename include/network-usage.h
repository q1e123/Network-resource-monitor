#ifndef NETWORKUSAGE_H
#define NETWORKUSAGE_H

#include<cstdio>

class Network_Usage{
public:
	Network_Usage(size_t = 0, size_t = 0);

	size_t get_rx();
	size_t get_tx();

	void set_rx(size_t);
	void set_tx(size_t);
	
	Network_Usage operator-(Network_Usage &nu);
private:
	size_t rx,tx;
};

#endif
