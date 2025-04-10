#include "memory_bus.h"

#include <stdlib.h>

static u8 system_memory[SYSTEM_MEMORY_SIZE];
static u8 cartridge_space[CARTRIDGE_SIZE];
static u8 video_memory[VIDEO_MEMORY_SIZE];

void memory_init() {
	for (u16 i = 0; i < SYSTEM_MEMORY_SIZE; i++) {
		system_memory[i] = 0x00;
	}

	for (u16 i = 0; i < CARTRIDGE_SIZE; i++) {
		cartridge_space[i] = 0x00;
	}

	for (u16 i = 0; i < VIDEO_MEMORY_SIZE; i++) {
		video_memory[i] = 0x00;
	}
}

void memory_write(u16 location, u8 data) {
	if (location < SYSTEM_MEMORY_SIZE * 4) {
		system_memory[location % SYSTEM_MEMORY_SIZE] = data;
	}
	else if (location >= 0x2000 && location <= 0x2007) {
		// PPU Registers
	}
	else if (location >= 0x4000 && location <= 0x4017) {
		// APU and I/O Registers
	}
	else if (location >= 0x4018 && location <= 0x401F) {
		// APU and I/O functionality that is disabled normally
	}
	else { // 0x4020-0xFFFF
		// Cartridge
	}
}

u8 memory_read(u16 location) {
	if (location < SYSTEM_MEMORY_SIZE * 4) {
		return system_memory[location % SYSTEM_MEMORY_SIZE];
	}
	else if (location >= 0x2000 && location <= 0x2007) {
		// PPU Registers
		return 0x00;
	}
	else if (location >= 0x4000 && location <= 0x4017) {
		// APU and I/O Registers
		return 0x00;
	}
	else if (location >= 0x4018 && location <= 0x401F) {
		// APU and I/O functionality that is disabled normally
		return 0x00;
	}
	else { // 0x4020-0xFFFF
		// Cartridge
		return 0x00;
	}
}

void video_memory_write(u16 location, u8 data) {
	if (location < VIDEO_MEMORY_SIZE) {
		video_memory[location] = data;
	}
}

u8 video_memory_read(u16 location) {
	if (location < VIDEO_MEMORY_SIZE) {
		return video_memory[location];
	}
	else {
		return 0x00;
	}
}