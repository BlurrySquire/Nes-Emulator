#include "memory_bus.h"

#include <stdlib.h>

static u8 memory[MEMORY_MAX];

void memory_init() {
	for (u16 i = 0; i < MEMORY_MAX; i++) {
		memory[i] = 0x00;
	}
}

void memory_write(u16 location, u8 data) {
	if (location < MEMORY_MAX) {
		memory[location] = data;
	}
}

u8 memory_read(u16 location) {
	if (location < MEMORY_MAX) {
		return memory[location];
	}
	else {
		return 0x00;
	}
}