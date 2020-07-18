#include "network-usage.h"

Network_Usage::Network_Usage(size_t rx, size_t tx){
	this->rx = rx;
	this->tx = tx;
}

size_t Network_Usage::get_rx(){
	return rx;
}

size_t Network_Usage::get_tx(){
	return tx;
}

void Network_Usage::set_rx(size_t rx){
	this->rx = rx;
}

void Network_Usage::set_tx(size_t tx){
	this->tx = tx;
}

Network_Usage Network_Usage::operator-(Network_Usage &nu){
	Network_Usage tmp;
	tmp.set_rx(this->rx - nu.rx);
	tmp.set_tx(this->tx - nu.tx);
	return tmp;
}
