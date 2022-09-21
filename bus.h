#ifndef BUS_H
#define BUS_H

#include <iostream>
#include <thread>

#include <nlohmann/json.hpp>

#include "serial.h"
#include "message.h"

using json = nlohmann::json;

class Bus {
	private:
	Serial		*serial;
	json& 		busconfig;
	std::thread	*reader;


	void Reader();
	public:
	Bus(json& busconfig);
	~Bus();
};

#endif
