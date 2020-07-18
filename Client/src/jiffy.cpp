#include "jiffy.h"

Jiffy::Jiffy(size_t total, size_t work){
	this->total = total;
	this->work = work;
}

size_t Jiffy::get_total(){
	return total;
}
size_t Jiffy::get_work(){
	return work;
}

void Jiffy::set_total(size_t total){
	this->total = total;
}
void Jiffy::set_work(size_t work){
	this->work = work;
}

ostream& operator<<(ostream& os, const Jiffy &jiffy ){
	os<<jiffy.total<<" "<<jiffy.work;
    return os;
}
