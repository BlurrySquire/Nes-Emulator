#pragma once

#include <cstdint>
#include <array>

#include "cpu.hpp"

class MemoryBus {
public:
	MemoryBus();
	~MemoryBus();

	CPU cpu;
	std::array<uint8_t, 2048> ram;

	void Write(uint16_t address, uint8_t value);
	uint8_t Read(uint16_t address);
};