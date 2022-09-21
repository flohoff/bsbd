#ifndef FRAME_H
#define FRAME_H

#include <iostream>
#include <string>
#include "hexdump.hpp"
#include "crc.h"

class DeFramer {
	public:
		virtual bool findframe(const char c) {	
			return false;
		}
};

#define BSB_STARTOFFRAME	0xdc

class DeFramerBSB : public DeFramer {
	std::string	buffer;
	public:
		bool findframe(const char c) {

			// FIXME - Config issue?
			buffer+=c^0xff;

			if (buffer.at(0) != (char) BSB_STARTOFFRAME) {
				buffer.erase(0,1);
				return false;
			}

			if (!complete()) {
				return false;
			}

			uint16_t crc=CRC::poly1021((const uint8_t *) buffer.c_str(), framelength());	

			if (crc != 0) {
				std::cout << "CRC16 error - expected 0 - got " << crc << " dropping" << std::endl;
				hex::dump(buffer.c_str(), framelength(), std::cout);
				buffer.erase(0,framelength());
				return false;
			}

			return true;
		}

		std::string frame(void ) {
			std::string	frame=buffer.substr(0,framelength());
			buffer.erase(0, framelength());
			return frame;
		}

	private:
		int framelength() {
			return buffer.at(3);
		}

		bool complete() {
			if (buffer.length() < 4)
				return false;

			if (buffer.length() >= framelength()) 
				return true;
		
			return false;
		}
};


#endif
