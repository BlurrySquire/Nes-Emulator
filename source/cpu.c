#include "cpu.h"

#include "memory_bus.h"

void opcode_nop(cpu* state);
void opcode_lda(cpu* state, u8 operand);

u8 addressing_immediate(cpu* state);
u8 addressing_zeropage(cpu* state);
u8 addressing_zeropagex(cpu* state);
u8 addressing_zeropagey(cpu* state);
u8 addressing_absolute(cpu* state);
u8 addressing_absolutex(cpu* state);
u8 addressing_absolutey(cpu* state);
u8 addressing_indexedindirect(cpu* state);
u8 addressing_indirectindexed(cpu* state);

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
	state->current_instruction_cycles += 1;

	switch (instruction) {
		case 0xEA: opcode_nop(state); break;

		case 0xA9: opcode_lda(state, addressing_immediate(state)); break;
		case 0xA5: opcode_lda(state, addressing_zeropage(state)); break;
		case 0xB5: opcode_lda(state, addressing_zeropagex(state)); break;
		case 0xAD: opcode_lda(state, addressing_absolute(state)); break;
		case 0xBD: opcode_lda(state, addressing_absolutex(state)); break;
		case 0xB9: opcode_lda(state, addressing_absolutey(state)); break;
		case 0xA1: opcode_lda(state, addressing_indexedindirect(state)); break;
		case 0xB1: opcode_lda(state, addressing_indirectindexed(state)); break;
	}

	state->total_cycles += state->current_instruction_cycles;
}

void opcode_nop(cpu* state) {
	state->current_instruction_cycles += 1;
}

void opcode_lda(cpu* state, u8 operand) {
	state->accumulator = operand;

	state->register_status.zero_flag = !state->accumulator;
	state->register_status.negative_flag = state->accumulator & (1 << 7);
}

u8 addressing_immediate(cpu* state) {
	u8 byte = memory_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles += 1;
	
	return byte;
}

u8 addressing_zeropage(cpu* state) {
	u8 address = memory_read(state->program_counter);
	state->program_counter++;

	u8 byte = memory_read(address);
	state->current_instruction_cycles += 2;

	return byte;
}

u8 addressing_zeropagex(cpu* state) {
	u8 address = memory_read(state->program_counter) + state->register_x;
	state->program_counter++;

	u8 byte = memory_read(address);
	state->current_instruction_cycles += 3;

	return byte;
}

u8 addressing_zeropagey(cpu* state) {
	u8 address = memory_read(state->program_counter) + state->register_y;
	state->program_counter++;

	u8 byte = memory_read(address);
	state->current_instruction_cycles += 3;

	return byte;
}

u8 addressing_absolute(cpu* state) {
	u16 address = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
	state->program_counter += 2;
	
	u8 byte = memory_read(address);
	state->current_instruction_cycles += 3;

	return byte;
}

u8 addressing_absolutex(cpu* state) {
	u16 base = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
	u16 address = base + state->register_x;
	state->program_counter += 2;
	
	u8 byte = memory_read(address);
	state->current_instruction_cycles += 3;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles++;
	}

	return byte;
}

u8 addressing_absolutey(cpu* state) {
	u16 base = memory_read(state->program_counter) | (memory_read(state->program_counter + 1) << 8);
	u16 address = base + state->register_y;
	state->program_counter += 2;
	
	u8 byte = memory_read(address);
	state->current_instruction_cycles += 3;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles++;
	}

	return byte;
}

u8 addressing_indexedindirect(cpu* state) {
	u8 pointer = memory_read(state->program_counter) + state->register_x;
	state->program_counter++;

	u8 address = memory_read(pointer) | (memory_read(pointer + 1) << 8);
	u8 byte = memory_read(address);
	state->current_instruction_cycles += 5;

	return byte;
}

u8 addressing_indirectindexed(cpu* state) {
	u8 pointer = memory_read(state->program_counter);
	state->program_counter++;

	u16 base = memory_read(pointer) | (memory_read(pointer + 1) << 8);
	u16 address = base + state->register_y;
	u8 byte = memory_read(address);

	state->current_instruction_cycles += 4;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles++;
	}

	return byte;
}