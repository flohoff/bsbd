#ifndef VTYPE_H
#define VTYPE_H

#include <bits/stdint-uintn.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "hexdump.hpp"


class VType {
	protected:
	int		size;
	public:
		VType(int size) : size(size) {}
		virtual std::string decode(std::string value) const {
				std::stringstream ss;
				hex::dump(value.c_str(), value.size(), ss);
				return ss.str();
		}
};

class VTypeInt : public VType {
	std::string	unit;
	public:
		VTypeInt(int size, std::string unit) : VType(size),unit(unit) {};
};

class VTypeFloat : public VType {
	int	divisor;
	std::string	unit;
	public:
		VTypeFloat(int size, std::string unit, int divisor) : VType(size),unit(unit),divisor(divisor) {};
		std::string decode(std::string value) const {
			uint32_t	num=0;

			for(int i=0;i<size;i++) 
				num=(num<<8)|(uint8_t) value.at(i);

			std::stringstream ss;

			ss << std::fixed << std::showpoint << std::setprecision(2) << ((double) num)/divisor << unit;

			return ss.str();
		}
};

class VTypeBit : public VType {
	std::string	set;
	std::string	unset;
	public:
		VTypeBit(int size, std::string set, std::string unset) : VType(size),set(set),unset(unset) {};
		std::string decode(std::string value) const {
			if (value.at(0))
				return set;
			return unset;
		}
};


class VTypeUnknown : public VType {
	public:
		VTypeUnknown() : VType(0) {};
};

class VTypeDateTime : public VType {
	public:
		VTypeDateTime(int size) : VType(size) {};
};

#endif
