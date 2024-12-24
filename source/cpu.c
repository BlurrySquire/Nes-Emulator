#include "cpu.h"

#include "memory_bus.h"

void opcode_nop_implied(cpu* state);
void opcode_lda_immediate(cpu* state);
void opcode_ldx_immediate(cpu* state);
void opcode_ldy_immediate(cpu* state);

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
	state->current_instruction_cycles++;

	switch (instruction) {
		case 0xEA: opcode_nop_implied(state); break;

		// LDA - Load accumulator
		case 0xA9: opcode_lda_immediate(state); break;
		case 0xA5: break; // Zero page
		case 0xB5: break; // Zero page, X
		case 0xAD: break; // Absolute
		case 0xBD: break; // Absolute, X
		case 0xB9: break; // Absolute, Y
		case 0xA1: break; // (Indirect, X)
		case 0xB1: break; // (Indirect), Y
		
		// LDX - Load register X
		case 0xA2: opcode_ldx_immediate(state); break;

		// LDY - Load register Y
		case 0xA0: opcode_ldy_immediate(state); break;
	}

	state->total_cycles += state->current_instruction_cycles;
}

void opcode_nop_implied(cpu* state) {
	state->current_instruction_cycles++;
}

void opcode_lda_immediate(cpu* state) {
	state->accumulator = memory_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles++;

	state->register_status.zero_flag = !state->accumulator;
	state->register_status.negative_flag = state->accumulator & (1 << 7);
}

void opcode_ldx_immediate(cpu* state) {
	state->register_x = memory_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles++;

	state->register_status.zero_flag = !state->register_x;
	state->register_status.negative_flag = state->register_x & (1 << 7);
}

void opcode_ldy_immediate(cpu* state) {
	state->register_y = memory_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles++;

	state->register_status.zero_flag = !state->register_y;
	state->register_status.negative_flag = state->register_y & (1 << 7);
}