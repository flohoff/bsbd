#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>
#include <bits/stdint-uintn.h>
#include <string>
#include <map>

#include "AddressMap.h"
#include "hexdump.hpp"

typedef enum {
	MT_QINF = 0x2,
	MT_SET = 0x3,
	MT_ACK = 0x4,
	MT_NACK = 0x5,
	MT_QUERY = 0x6,
	MT_RESPONSE = 0x7,
	MT_ERROR = 0x8
} MType;

static const std::map<int,std::string> messagetypestringtable = {
	{ MT_QINF, "Query info" },
	{ MT_SET, "Set" },
	{ MT_ACK, "Ack" },
	{ MT_NACK, "Nack" },
	{ MT_QUERY, "Query" },
	{ MT_RESPONSE, "Response" },
	{ MT_ERROR, "Error" }
};

class Message {
	std::string message;
	public:
		Message(std::string message) : message(message) {}

		// DC 8A 00 0B 06 3D 0D 05 19 4F 8C
		uint32_t address(void ) const {
			uint32_t	addr=0;

			for(int i=0;i<=3;i++) {
				addr<<=8;
				addr|=message.at(i+5);
			}

			// Query Swaps the first 2 bytes
			if (needs_swap()) {
				addr=((addr&0xff000000)>>8)|((addr&0x00ff0000)<<8)|(addr&0x0000ffff);
			}

			return addr;
		}

		bool needs_swap(void ) const {
			if (messagetype() == MT_QUERY)
				return true;
			if (messagetype() == MT_SET)
				return true;
			return false;
		}

		unsigned int messagetype(void ) const {
			return (uint8_t) message.at(4);
		}

		std::string messagetypestring(void ) const {
			return messagetypestringtable.at(messagetype());
		}

		std::string value(void ) const {
			std::string value=message.substr(9,message.size()-11);
			return value;
		}

		bool hasvalue(void ) const {
			if (messagetype() == MT_RESPONSE
				|| messagetype() == MT_QINF)
				return true;
			return false;
		}

		unsigned int to(void ) const {
			return (uint8_t) message.at(2);
		}

		unsigned int from(void ) const {
			return (uint8_t) message.at(1);
		}

		friend std::ostream& operator<<(std::ostream& os, const Message& m) {
			Address *aobj=addressmap.find(m.address());
			os << "From:    " << std::hex << m.from() << std::endl;
			os << "To:      " << std::hex << m.to() << std::endl;
			os << "Type:    " << std::hex << m.messagetypestring() << "(" << m.messagetype() << ")" << std::endl;
			if (aobj == nullptr) {
				os << "Address: " << "Unknown" << "(" << std::hex << m.address() << ")" << std::endl;
			} else {
				os << "Address: " << aobj->name << "(" << std::hex << m.address() << ")" << std::endl;
				if (m.hasvalue()) {
					os << "Value:   " << aobj->vtype->decode(m.value()) << std::endl;
				}
			}
			hex::dump(m.message.c_str(), m.message.size(), os);
			return os;
		}
};
#endif
