#ifndef CRC_H
#define CRC_H

#include <cstdint>

class CRC {
	public:
	static uint16_t poly1021(const uint8_t *ptr, int count);
};

#endif
