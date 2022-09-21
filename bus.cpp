#include <iostream>

#include "bus.h"
#include "frame.h"
#include "message.h"

Bus::Bus(json& busconfig) : busconfig(busconfig) {
	// FIXME - Parameter checking?
	serial=new Serial(busconfig["device"].get<std::string>(),
			busconfig["baudrate"].get<std::string>(),
			busconfig["mode"].get<std::string>());

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
