#ifndef JIFFY_H
#define JIFFY_H

#include <cstdio>
#include <iostream>

class Jiffy{
public:
	Jiffy(size_t=0, size_t=0);	
	size_t get_total();
	size_t get_work();
	
	void set_total(size_t);
	void set_work(size_t);

	friend std::ostream& operator<<(std::ostream& os, const Jiffy&);
private:
	size_t total, work;
};
#endif
