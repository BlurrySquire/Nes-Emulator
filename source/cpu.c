#include "cpu.h"

#include "memory_bus.h"

typedef enum addressing_mode {
	IMPLIED,
	ACCUMULATOR,
	IMMEDIATE,
	ZEROPAGE, ZEROPAGEX, ZEROPAGEY,
	RELATIVE,
	ABSOLUTE, ABSOLUTEX, ABSOLUTEY,
	INDIRECT, INDEXEDINDIRECT, INDIRECTINDEXED
} addressing_mode;

void opcode_nop(cpu* state, addressing_mode mode);

void opcode_lda(cpu* state, addressing_mode mode);

void cpu_init(cpu* state) {
	state->total_cycles = 0;
	state->current_instruction_cycles = 0;
	
	state->program_counter = 0x0000;
	state->stack_pointer = 0x00;

	state->accumulator = 0x00;
	state->register_x = 0x00;
	state->register_y = 0x00;

	state->register_status.carry_flag = 0;
	state->register_status.zero_flag = 0;
	state->register_status.interrupt_disable = 0;
	state->register_status.decimal_flag = 0;
	state->register_status.overflow_flag = 0;
	state->register_status.negative_flag = 0;
	state->register_status.the_b_flag = 0;
}

void cpu_execute_instruction(cpu* state) {
	u8 instruction = memory_read(state->program_counter);
	state->program_counter++;

	switch (instruction) {
		case 0xEA: opcode_nop(state, IMPLIED); break;

		// LDA - Load accumulator
		case 0xA9: opcode_lda(state, IMMEDIATE); break;
		case 0xA5: opcode_lda(state, ZEROPAGE); break;
		case 0xB5: opcode_lda(state, ZEROPAGEX); break;
		case 0xAD: opcode_lda(state, ABSOLUTE); break;
		case 0xBD: opcode_lda(state, ABSOLUTEX); break;
		case 0xB9: opcode_lda(state, ABSOLUTEY); break;
		case 0xA1: opcode_lda(state, INDEXEDINDIRECT); break;
		case 0xB1: opcode_lda(state, INDIRECTINDEXED); break;
	}

	state->total_cycles += state->current_instruction_cycles;
}

void opcode_nop(cpu* state, addressing_mode mode) {
	switch (mode) {
		case IMPLIED: {
			state->current_instruction_cycles = 2;
		} break;
	}

	state->current_instruction_cycles = 2;
}

void opcode_lda(cpu* state, addressing_mode mode) {
	switch (mode) {
		case IMMEDIATE: {
			state->accumulator = memory_read(state->program_counter);
			state->program_counter++;

			state->current_instruction_cycles = 2;
		} break;

		case ZEROPAGE: {
			u8 address = memory_read(state->program_counter);
			state->accumulator = memory_read(address);
			state->program_counter++;

			state->current_instruction_cycles = 3;
		} break;

		case ZEROPAGEX: {
			u8 address = memory_read(state->program_counter) + state->register_x;
			state->accumulator = memory_read(address);
			state->program_counter++;

			state->current_instruction_cycles = 4;
		} break;

		case ABSOLUTE: {
			u16 address = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
			state->program_counter += 2;
			state->accumulator = memory_read(address);

			state->current_instruction_cycles = 4;
		} break;

		case ABSOLUTEX: {
			u16 base = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
			u16 address = base + state->register_x;
			state->program_counter += 2;

			state->accumulator = memory_read(address);

			state->current_instruction_cycles = 4;
			if ((base & 0xFF00) != (address & 0xFF00)) {
				state->current_instruction_cycles++; // Extra cycle if crossing a page boundary
			}
		} break;

		case ABSOLUTEY: {
			u16 base = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
			u16 address = base + state->register_y;
			state->program_counter += 2;

			state->accumulator = memory_read(address);

			state->current_instruction_cycles = 4;
			if ((base & 0xFF00) != (address & 0xFF00)) {
				state->current_instruction_cycles++; // Extra cycle if crossing a page boundary
			}
		} break;

		case INDEXEDINDIRECT: {
			u8 pointer = memory_read(state->program_counter) + state->register_x;
			state->program_counter++;

			u8 address = memory_read(pointer) | (memory_read(pointer + 1) << 8);
			state->accumulator = memory_read(address);

			state->current_instruction_cycles = 6;
		} break;

		case INDIRECTINDEXED: {
			u8 pointer = memory_read(state->program_counter);
			state->program_counter++;

			u16 base = memory_read(pointer) | (memory_read(pointer + 1) << 8);
			u16 address = state->register_y;
			state->accumulator = memory_read(address);

			state->current_instruction_cycles = 5;
			if ((base & 0xFF00) != (address & 0xFF00)) {
				state->current_instruction_cycles++;
			}
		} break;
	}

	state->register_status.zero_flag = !state->accumulator;
	state->register_status.negative_flag = state->accumulator & (1 << 7);
}