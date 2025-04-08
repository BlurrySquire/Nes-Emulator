#include "addressing_modes.h"

u16 addressing_immediate(cpu* state) {
	return state->program_counter++;
}

u16 addressing_zeropage(cpu* state) {
	u8 address = memory_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles += 1;

	return address;
}

u16 addressing_zeropagex(cpu* state) {
	u8 address = memory_read(state->program_counter) + state->register_x;
	state->program_counter++;
	state->current_instruction_cycles += 2;

	return address;
}

u16 addressing_zeropagey(cpu* state) {
	u8 address = memory_read(state->program_counter) + state->register_y;
	state->program_counter++;
	state->current_instruction_cycles += 2;

	return address;
}

u16 addressing_absolute(cpu* state) {
	u16 address = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
	state->program_counter += 2;
	state->current_instruction_cycles += 2;

	return address;
}

u16 addressing_absolutex(cpu* state) {
	u16 base = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
	u16 address = base + state->register_x;
	state->program_counter += 2;
	state->current_instruction_cycles += 3;

	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles += 1;
	}

	return address;
}

u16 addressing_absolutey(cpu* state) {
	u16 base = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
	u16 address = base + state->register_y;
	state->program_counter += 2;
	state->current_instruction_cycles += 3;

	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles += 1;
	}

	return address;
}

u16 addressing_indirect(cpu* state) {
	u16 pointer = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);

	u8 low = memory_read(pointer);
	u8 high;

	// Bug where if low byte on page boundary then high byte wraps around to page start
	if ((pointer & 0x00FF) == 0x00FF) {
		high = memory_read(pointer & 0xFF00);
	}
	else {
		high = memory_read(pointer + 1);
	}

	return (high << 8) | low;
}

u16 addressing_indexedindirect(cpu* state) {
	u8 pointer = memory_read(state->program_counter) + state->register_x;
	state->program_counter++;

	u16 address = memory_read(pointer) | (memory_read(pointer + 1) << 8);
	state->current_instruction_cycles += 4;

	return address;
}

u16 addressing_indirectindexed(cpu* state) {
	u8 pointer = memory_read(state->program_counter);
	state->program_counter++;

	u16 base = memory_read(pointer) | (memory_read(pointer + 1) << 8);
	u16 address = base + state->register_y;

	state->current_instruction_cycles += 4;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles += 1;
	}

	return address;
}