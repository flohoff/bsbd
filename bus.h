#ifndef BUS_H
#define BUS_H

#include <iostream>
#include <thread>

#include "serial.h"
#include "config.h"
#include "message.h"

class Bus {
	private:
	Serial		*serial;
	config::bus	config;
	std::thread	*reader;

	void Reader();
	public:
	Bus(config::bus& config);
	~Bus();
};

#endif
