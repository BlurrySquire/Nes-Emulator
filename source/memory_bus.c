#include "memory_bus.h"
#include "cartridge.h"

static u8 cpu_memory[0xFFFF];
static u8 ppu_memory[0x07FF];

void cpubus_init() {
	for (u16 i = 0; i < 0x07FF; i++) {
		cpu_memory[i] = 0x00;
	}
}

u8 cpubus_read(u16 address) {
	return cpu_memory[address];
}

void cpubus_write(u16 address, u8 value) {
	cpu_memory[address] = value;
}

/*
u8 cpubus_read(u16 address) {
	// 0x0000-0x1FFF CPU RAM
	if ((address & 0x07FF) < 0x07FF && address < 0x1FFF) {
		return cpu_memory[address & 0x07FF];
	}
	// 0x2000-0x3FFF PPU Registers
	else if (address >= 0x2000 && address <= 0x3FFF) {
		return 0x00;
	}
	// 0x4000-0x4017 APU & I/O Registers
	else if (address >= 0x4000 && address <= 0x4017) {
		return 0x00;
	}
	// 0x4018-0x401F APU & I/O functionality from test mode
	else if (address >= 0x4018 && address <= 0x401F) {
		return 0x00;
	}
	// 0x4020-0xFFFF Cartridge use
	else {
		return cartridge_read(address - 0x4020);
	}
}

void cpubus_write(u16 address, u8 value) {
	// 0x0000-0x1FFF CPU RAM
	if ((address & 0x07FF) < 0x07FF && address < 0x1FFF) {
		cpu_memory[address & 0x07FF] = value;
	}
	// 0x2000-0x3FFF PPU Registers
	else if (address >= 0x2000 && address <= 0x3FFF) {
		// Write to PPU Registers
	}
	// 0x4000-0x4017 APU & I/O Registers
	else if (address >= 0x4000 && address <= 0x4017) {
		// Write to APU or I/O Registers
	}
	// 0x4018-0x401F APU & I/O functionality from test mode
	else if (address >= 0x4018 && address <= 0x401F) {
		// test mode
	}
	// 0x4020-0xFFFF Cartridge use
	else {
		cartridge_write(address - 0x4020, value);
	}
}
*/

void ppubus_init() {
	for (u16 i = 0; i < 0x07FF; i++) {
		ppu_memory[i] = 0x00;
	}
}

u8 ppubus_read(u16 address) {
	if (address < 0x07FF) {
		return ppu_memory[address];
	}
	else {
		return 0x00;
	}
}

void ppubus_write(u16 address, u8 value) {
	if (address < 0x07FF) {
		ppu_memory[address] = value;
	}
}