#include "cpu.h"

#include "memory_bus.h"
#include "addressing_modes.h"
#include "opcodes.h"

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

		case 0xA2: opcode_ldx(state, addressing_immediate(state)); break;
		case 0xA6: opcode_ldx(state, addressing_zeropage(state)); break;
		case 0xB6: opcode_ldx(state, addressing_zeropagey(state)); break;
		case 0xAE: opcode_ldx(state, addressing_absolute(state)); break;
		case 0xBE: opcode_ldx(state, addressing_absolutey(state)); break;

		case 0xA0: opcode_ldy(state, addressing_immediate(state)); break;
		case 0xA4: opcode_ldy(state, addressing_zeropage(state)); break;
		case 0xB4: opcode_ldy(state, addressing_zeropagex(state)); break;
		case 0xAC: opcode_ldy(state, addressing_absolute(state)); break;
		case 0xBC: opcode_ldy(state, addressing_absolutex(state)); break;

		case 0x4C: opcode_jmp(state, addressing_absolute(state)); break;
		case 0x6C: opcode_jmp(state, addressing_indirect(state)); break;
	}

	state->total_cycles += state->current_instruction_cycles;
}