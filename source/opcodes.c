#include "opcodes.h"
#include "memory_bus.h"

//
// ACCESS
//

void opcode_lda(cpu* state, u16 address) {
	state->accumulator = memory_read(address);
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = state->accumulator & (1 << 7);
}

void opcode_sta(cpu* state, u16 address) {
	memory_write(address, state->accumulator);
	state->current_instruction_cycles += 1;
}

void opcode_ldx(cpu* state, u16 address) {
	state->register_x = memory_read(address);
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = state->register_x & (1 << 7);
}

void opcode_stx(cpu* state, u16 address) {
	memory_write(address, state->register_x);
	state->current_instruction_cycles += 1;
}

void opcode_ldy(cpu* state, u16 address) {
	state->register_y = memory_read(address);
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = state->register_y & (1 << 7);
}

void opcode_sty(cpu* state, u16 address) {
	memory_write(address, state->register_y);
	state->current_instruction_cycles += 1;
}

//
// TRANSFER
//

void opcode_tax(cpu* state) {
	state->register_x = state->accumulator;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = state->register_y & (1 << 7);
}

void opcode_tay(cpu* state) {
	state->register_y = state->accumulator;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = state->register_y & (1 << 7);
}

void opcode_txa(cpu* state) {
	state->accumulator = state->register_x;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = state->register_y & (1 << 7);
}

void opcode_tya(cpu* state) {
	state->accumulator = state->register_y;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = state->register_y & (1 << 7);
}

//
// ARITHMETIC
//

void opcode_adc(cpu* state, u16 address) {
	u8 memory = memory_read(address);
	u16 result = state->accumulator + memory + state->status.carry_flag;
	state->current_instruction_cycles += 1;

	state->status.carry_flag = result > 0x00FF;
	state->status.zero_flag = !result;
	state->status.overflow_flag = (result ^ state->accumulator) & (result ^ memory) & 0x80;
	state->status.negative_flag = result & (1 << 7);

	state->accumulator = result & 0xFF;
}

void opcode_sbc(cpu* state, u16 address) {
	u8 memory = memory_read(address);
	u16 result = state->accumulator - memory - (1 - state->status.carry_flag);
	state->current_instruction_cycles += 1;

	state->status.carry_flag = result > 0x00FF;
	state->status.zero_flag = !result;
	state->status.overflow_flag = (result ^ state->accumulator) & (result ^ memory) & 0x80;
	state->status.negative_flag = result & (1 << 7);

	state->accumulator = result & 0xFF;
}

void opcode_inc(cpu* state, u16 address) {
	u8 value = memory_read(address);
	u8 result = value + 1;

	memory_write(address, value);
	memory_write(address, result);
	state->current_instruction_cycles += 3;

	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);
}

void opcode_dec(cpu* state, u16 address) {
	u8 value = memory_read(address);
	u8 result = value - 1;

	memory_write(address, value);
	memory_write(address, result);
	state->current_instruction_cycles += 3;

	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);
}

void opcode_inx(cpu* state) {
	state->register_x += 1;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = state->register_x & (1 << 7);
}

void opcode_dex(cpu* state) {
	state->register_x -= 1;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = state->register_x & (1 << 7);
}

void opcode_iny(cpu* state) {
	state->register_y += 1;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = state->register_x & (1 << 7);
}

void opcode_dey(cpu* state) {
	state->register_y -= 1;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = state->register_x & (1 << 7);
}

//
// STACK
//

void opcode_pha(cpu* state) {
	memory_write(state->stack_pointer, state->accumulator);
	state->stack_pointer--;

	state->current_instruction_cycles += 1;
}

void opcode_pla(cpu* state) {
	state->accumulator = memory_read(state->stack_pointer);
	state->stack_pointer++;

	state->current_instruction_cycles += 1;
}

void opcode_php(cpu* state) {
	memory_write(state->stack_pointer, state->status.as_byte);
	state->stack_pointer--;

	state->current_instruction_cycles += 1;
}

void opcode_plp(cpu* state) {
	state->status.as_byte = memory_read(state->stack_pointer);
	state->stack_pointer++;

	state->current_instruction_cycles += 1;
}

void opcode_tsx(cpu* state) {
	state->register_x = state->stack_pointer;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = state->register_y & (1 << 7);
}

void opcode_txs(cpu* state) {
	state->stack_pointer = state->register_x;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = state->register_y & (1 << 7);
}

//
// JUMP
//

void opcode_jmp(cpu* state, u16 address) {
	state->program_counter = address;
}

//
// OTHER
//

void opcode_nop(cpu* state) {
	state->current_instruction_cycles += 1;
}

//
// FLAGS
//

void opcode_clc(cpu* state) {
	state->status.carry_flag = 0;
	state->current_instruction_cycles += 1;
}

void opcode_sec(cpu* state) {
	state->status.carry_flag = 1;
	state->current_instruction_cycles += 1;
}

void opcode_cli(cpu* state) {
	state->interrupt_flag = 0;
	state->instruction_delay = 1;
	state->current_instruction_cycles += 1;
}

void opcode_sei(cpu* state) {
	state->interrupt_flag = 1;
	state->instruction_delay = 1;
	state->current_instruction_cycles += 1;
}

void opcode_cld(cpu* state) {
	state->status.decimal_flag = 0;
	state->current_instruction_cycles += 1;
}

void opcode_sed(cpu* state) {
	state->status.decimal_flag = 1;
	state->current_instruction_cycles += 1;
}

void opcode_clv(cpu* state) {
	state->status.overflow_flag = 0;
	state->current_instruction_cycles += 1;
}