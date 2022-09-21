
#include "serial.h"


Serial::Serial(std::string port, std::string baud, std::string mode) : io(), serial(io,port) {
	this->setoption(baud, mode);
}

void Serial::setoption(std::string& baud, std::string& mode) {

	std::cout << "Mode " << mode << std::endl;

	/* Mode 7e1 8n1 8n2 */
	char bits=mode.at(0);
	if (bits == '7') {
		serial.set_option(boost::asio::serial_port_base::character_size(7));
	} else {
		serial.set_option(boost::asio::serial_port_base::character_size(8));
	}

	char parity=mode.at(1);
	std::cout << "Parity " << parity << std::endl;
	if (parity == 'e') {
		std::cout << "Parity setting even" << std::endl;
		serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::even));
	} else if (parity == 'o') {
		serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::odd));
	} else {
		serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
	}

	// FIXME Stop bits?
	serial.set_option(boost::asio::serial_port_base::baud_rate(std::stoi(baud)));
}

void Serial::write(std::string s) {
	boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
}

// Read until we have a complete frame - then return
void Serial::read(DeFramer& df) {
	char c;
	for(;;) {
		boost::asio::read(serial,boost::asio::buffer(&c,1));
		if (df.findframe(c))
			break;
	}
}

