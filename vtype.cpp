
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

#include "hexdump.hpp"
#include "vtype.h"

std::map<std::string,VT::Default*>  vtmap;

namespace VT {

Default::Default(std::string name, Default *vt) {
	vtmap.emplace(name, vt);
}

std::string Default::decode(std::string value) const {
	std::stringstream ss;
	hex::dump(value.c_str(), value.size(), ss);
	return ss.str();
}

class Float : public Default {
    int size;
    std::string unit;
    int divisor;

    public:
        Float(std::string name, int size, std::string unit, int divisor) : Default(name,this),size(size),unit(unit),divisor(divisor) {}
    	std::string decode(std::string value) const {
	    	uint32_t	num=0;

			for(int i=0;i<size;i++) 
				num=(num<<8)|(uint8_t) value.at(i);

			std::stringstream ss;

			ss << std::fixed << std::showpoint << std::setprecision(2) << ((double) num)/divisor << unit;

			return ss.str();
		}
};

class Hexdump : public Default {
    public:
        Hexdump(std::string name) : Default(name, this) {}
};

class DateTime : public Default {
    public:
        DateTime(std::string name) : Default(name, this) {}
};

class Bit : public Default {
	std::string	set;
	std::string	unset;
	public:
		Bit(std::string name, int size, std::string set, std::string unset) : Default(name, this),set(set),unset(unset) {};
		std::string decode(std::string value) const {
			if (value.at(0))
				return set;
			return unset;
		}
};

class Enum : public Default {
    public:
        Enum(std::string name) : Default(name, this) {}
};

} // Namespace VT


static VT::Hexdump  hexdump("hexdump");
static VT::Hexdump  unknown("unknown");
static VT::Hexdump  hkinfo("hkinfo");
static VT::DateTime datetime("datetime");
static VT::Float    float3div64tempC("float3-div64-tempC", 3, "°C", 64);
static VT::Bit      bitanaus("bit-anaus", 1, "an", "aus");
static VT::Enum     enumlist("enum");
