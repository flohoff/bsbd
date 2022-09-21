#ifndef SERIAL_H
#define	SERIAL_H

#include <boost/asio.hpp>
#include "hexdump.hpp"

#include "frame.h"

class Serial {
	private:
		boost::asio::io_service io;
		boost::asio::serial_port serial;
		std::string inbuffer;

	private:
		void setoption(std::string& baud, std::string& mode);

	public:
		Serial(std::string port, std::string baud, std::string mode);
		void write(std::string s);
		void read(DeFramer& df);
};

#endif

