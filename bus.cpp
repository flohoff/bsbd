#include <iostream>

#include "bus.h"
#include "config.h"
#include "frame.h"
#include "message.h"

Bus::Bus(config::bus& config) : config(config) {
	// FIXME - Parameter checking?
	serial=new Serial(config.device, config.baudrate, config.mode);

	reader=new std::thread(&Bus::Reader, this);
}

void Bus::Reader() {
	DeFramerBSB	df;
	while(42) {
		serial->read(df);
		
		Message msg(df.frame());

		std::cout << msg << std::endl;
	}
}

Bus::~Bus() {
	reader->join();
	delete(serial);
}
