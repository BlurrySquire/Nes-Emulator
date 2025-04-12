#include "cartridge.h"

static u8 cartridge_data[0xBFE0];

void cartridge_init(char* rom_path) {
	for (u16 i = 0; i < 0xBFE0; i++) {
		cartridge_data[i] = 0x00;
	}
}

u8 cartridge_read(u16 address) {
	return cartridge_data[address];
}

void cartridge_write(u16 address, u8 value) {
	if (address <= 0x2000) {
		cartridge_data[address] = value;
	}

	// Remove the limit of only allowing writes to ram.
	// This is until cartridge stuff is complete.
	cartridge_data[address] = value;
}