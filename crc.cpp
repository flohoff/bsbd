
#include "crc.h"

uint16_t CRC::poly1021(const uint8_t *ptr, int count) {
	int  crc;
	char i;
	crc = 0;
	while (--count >= 0) {
		crc = crc ^ (int) *ptr++ << 8;
		i = 8;
		do {
			if (crc & 0x8000)
				crc = crc << 1 ^ 0x1021;
			else
				crc = crc << 1;
		} while(--i);
	}
	return (crc);
}
