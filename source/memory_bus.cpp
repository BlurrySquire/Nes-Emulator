#include "memory_bus.hpp"

MemoryBus::MemoryBus() {
	for (uint16_t i = 0; i < 2048; i++) {
		this->ram[i] = 0x00;
	}

	cpu.ConnectBus(this);
}

MemoryBus::~MemoryBus() {

}

void MemoryBus::Write(uint16_t address, uint8_t value) {
	if (address <= 0x2000) {
		this->ram[address] = value;
	}
}

uint8_t MemoryBus::Read(uint16_t address) {
	if (address <= 0x2000) {
		return this->ram[address];
	}
	else {
		return 0x00;
	}
}