#include "opcodes.h"

void opcode_nop(cpu* state) {
	state->current_instruction_cycles += 1;
}

void opcode_lda(cpu* state, u16 address) {
	state->accumulator = memory_read(address);
	state->current_instruction_cycles += 1;

	state->register_status.zero_flag = !state->accumulator;
	state->register_status.negative_flag = state->accumulator & (1 << 7);
}

void opcode_ldx(cpu* state, u16 address) {
	state->register_x = memory_read(address);
	state->current_instruction_cycles += 1;

	state->register_status.zero_flag = !state->register_x;
	state->register_status.negative_flag = state->register_x & (1 << 7);
}

void opcode_ldy(cpu* state, u16 address) {
	state->register_y = memory_read(address);
	state->current_instruction_cycles += 1;

	state->register_status.zero_flag = !state->register_y;
	state->register_status.negative_flag = state->register_y & (1 << 7);
}

void opcode_jmp(cpu* state, u16 address) {
	state->program_counter = address;
}