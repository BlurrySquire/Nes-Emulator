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
// SHIFT
//

void opcode_asl(cpu* state, u16 address) {
	u8 value = memory_read(address);
	u8 result = value << 1;

	memory_write(address, value);
	memory_write(address, result);

	state->status.carry_flag = value & (1 << 7);
	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);

	state->current_instruction_cycles += 3;
}

void opcode_asl_accumulator(cpu* state) {
	state->status.carry_flag = state->accumulator & (1 << 7);
	state->accumulator = state->accumulator << 1;
	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = state->accumulator & (1 << 7);

	state->current_instruction_cycles += 1;
}

void opcode_lsr(cpu* state, u16 address) {
	u8 value = memory_read(address);
	u8 result = value >> 1;

	memory_write(address, value);
	memory_write(address, result);

	state->status.carry_flag = value & 1;
	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);

	state->current_instruction_cycles += 3;
}

void opcode_lsr_accumulator(cpu* state) {
	u8 value = state->accumulator;
	u8 result = value >> 1;

	state->accumulator = result;

	state->status.carry_flag = value & 1;
	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);

	state->current_instruction_cycles += 3;
}

void opcode_rol(cpu* state, u16 address) {
	u8 value = memory_read(address);
	u8 result = (value << 1) | state->status.carry_flag;

	memory_write(address, value);
	memory_write(address, result);

	state->status.carry_flag = value & (1 << 7);
	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);

	state->current_instruction_cycles += 3;
}

void opcode_rol_accumulator(cpu* state) {
	u8 value = state->accumulator;
	u8 result = (value << 1) | state->status.carry_flag;

	state->accumulator = result;

	state->status.carry_flag = value & (1 << 7);
	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);
}

void opcode_ror(cpu* state, u16 address) {
	u8 value = memory_read(address);
	u8 result = (value >> 1) | (state->status.carry_flag << 7);

	memory_write(address, value);
	memory_write(address, result);

	state->status.carry_flag = value & 1;
	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);
}

void opcode_ror_accumulator(cpu* state) {
	u8 value = state->accumulator;
	u8 result = (value >> 1) | (state->status.carry_flag << 7);

	state->accumulator = result;

	state->status.carry_flag = value & 1;
	state->status.zero_flag = !result;
	state->status.negative_flag = result & (1 << 7);
}

//
// BITWISE
//

void opcode_and(cpu* state, u16 address) {
	state->accumulator = state->accumulator & memory_read(address);

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = state->accumulator & (1 << 7);

	state->current_instruction_cycles += 1;
}

void opcode_ora(cpu* state, u16 address) {
	state->accumulator = state->accumulator | memory_read(address);

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = state->accumulator & (1 << 7);

	state->current_instruction_cycles += 1;
}

void opcode_eor(cpu* state, u16 address) {
	state->accumulator = state->accumulator ^ memory_read(address);

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = state->accumulator & (1 << 7);

	state->current_instruction_cycles += 1;
}

void opcode_bit(cpu* state, u16 address) {
	u8 value = memory_read(address);

	state->status.zero_flag = !(state->accumulator & value);
	state->status.negative_flag = value & (1 << 7);
	state->status.overflow_flag = value & (1 << 6);

	state->current_instruction_cycles += 1;
}

//
// COMPARE
//

void opcode_cmp(cpu* state, u16 address) {
	u8 value = memory_read(address);

	state->status.carry_flag = state->accumulator >= value;
	state->status.zero_flag = state->accumulator == value;
	state->status.negative_flag = (state->accumulator - value) & (1 << 7);

	state->current_instruction_cycles += 1;
}

void opcode_cpx(cpu* state, u16 address) {
	u8 value = memory_read(address);

	state->status.carry_flag = state->register_x >= value;
	state->status.zero_flag = state->register_x == value;
	state->status.negative_flag = (state->register_x - value) & (1 << 7);

	state->current_instruction_cycles += 1;
}

void opcode_cpy(cpu* state, u16 address) {
	u8 value = memory_read(address);

	state->status.carry_flag = state->register_y >= value;
	state->status.zero_flag = state->register_y == value;
	state->status.negative_flag = (state->register_y - value) & (1 << 7);

	state->current_instruction_cycles += 1;
}

//
// JUMP
//

void opcode_jmp(cpu* state, u16 address) {
	state->program_counter = address;
}

void opcode_jsr(cpu* state, u16 address) {
	state->program_counter--;
	memory_write(state->stack_pointer + 0x0100, (state->program_counter & 0xFF00) >> 8);
	state->stack_pointer--;
	memory_write(state->stack_pointer + 0x0100, state->program_counter & 0x00FF);
	state->stack_pointer--;

	state->program_counter = address;

	state->current_instruction_cycles += 3;
}

void opcode_rts(cpu* state) {
	state->stack_pointer++;
	u8 low = memory_read(state->stack_pointer + 0x0100);
	state->stack_pointer++;
	u8 high = memory_read(state->stack_pointer + 0x0100);

	u16 address = (high << 8) | low;
	state->program_counter = address + 1;

	state->current_instruction_cycles += 3;
}

void opcode_brk(cpu* state) {
	state->program_counter++;
	memory_write(state->stack_pointer + 0x0100, (state->program_counter & 0xFF00) >> 8);
	state->stack_pointer--;
	memory_write(state->stack_pointer + 0x0100, state->program_counter & 0x00FF);
	state->stack_pointer--;

	state->status.break_flag = 1;
	memory_write(state->stack_pointer + 0x0100, state->status.as_byte);
	state->status.break_flag = 0;

	state->status.interrupt_disable = 1;

	u8 low = memory_read(0xFFFE);
	u8 high = memory_read(0xFFFF);
	state->program_counter = (low << 8) | high;

	state->current_instruction_cycles += 6;
}

void opcode_rti(cpu* state) {
	state->stack_pointer++;
	state->status.as_byte = memory_read(state->stack_pointer + 0x0100);

	state->stack_pointer++;
    u8 low = memory_read(state->stack_pointer + 0x0100);
    state->stack_pointer++;
    u8 high = memory_read(state->stack_pointer + 0x0100);

	state->program_counter = (high << 8) | low;
	state->current_instruction_cycles += 5;
}

//
// STACK
//

void opcode_pha(cpu* state) {
	memory_write(state->stack_pointer + 0x0100, state->accumulator);
	state->stack_pointer--;

	state->current_instruction_cycles += 1;
}

void opcode_pla(cpu* state) {
	state->accumulator = memory_read(state->stack_pointer + 0x0100);
	state->stack_pointer++;

	state->current_instruction_cycles += 1;
}

void opcode_php(cpu* state) {
	memory_write(state->stack_pointer + 0x0100, state->status.as_byte);
	state->stack_pointer--;

	state->current_instruction_cycles += 1;
}

void opcode_plp(cpu* state) {
	state->status.as_byte = memory_read(state->stack_pointer + 0x0100);
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