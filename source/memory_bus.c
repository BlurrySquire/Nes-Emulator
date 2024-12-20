#include "memory_bus.h"

#include <stdlib.h>

static u8 system_memory[SYSTEM_MEMORY_SIZE];
static u8 video_memory[VIDEO_MEMORY_SIZE];

void memory_init() {
	for (u16 i = 0; i < SYSTEM_MEMORY_SIZE; i++) {
		system_memory[i] = 0x00;
	}

	for (u16 i = 0; i < VIDEO_MEMORY_SIZE; i++) {
		video_memory[i] = 0x00;
	}
}

void memory_write(u16 location, u8 data) {
	if (location < SYSTEM_MEMORY_SIZE) {
		system_memory[location] = data;
	}
}

u8 memory_read(u16 location) {
	if (location < SYSTEM_MEMORY_SIZE) {
		return system_memory[location];
	}
	else {
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