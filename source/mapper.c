#include "mapper.h"

u8 mapper0_read(u16 address, u8* prg_ram, u8* prg_rom, u8 prg_rom_size) {
	if (address < 0x8000) {
		return prg_ram[address - 0x4020];
	}
	else {
		if (prg_rom_size == 2) {
			// 32k rom
			return prg_rom[address - 0x4020];
		}
		else {
			// 16k rom, needs mirroring
			return prg_rom[(address - 0x4020) & (16 * 1024)];
		}
	}
}

void mapper0_write(u16 address, u8 value, u8* prg_ram) {
	if (address < 0x8000) {
		prg_ram[address] = value;
	}
}