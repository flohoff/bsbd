#ifndef ADDRESSMAP_H
#define ADDRESSMAP_H

#include <stdint.h>
#include <map>
#include <string>
#include <stdexcept>

#include "vtype.h"


class Address {
    public:
	VT::Default	*vtype;
	uint32_t	address;
	std::string	name;

	public:
		Address(uint32_t address, VT::Default *vt, std::string& name) : address(address), vtype(vt), name(name) {
		}

};

class AddressMap {
    std::map<uint32_t, Address *> amap;
    public:
        AddressMap(void ) {};
        void add(Address *a) {
            amap.emplace(a->address, a);
        }
        Address *find(uint32_t addr) {
            try {
                return amap.at(addr);
            } catch(std::out_of_range e) {
                return nullptr;
            }
            return nullptr;
        }
};

extern AddressMap addressmap;

#endif