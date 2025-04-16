#include "cpu.h"

#include "memory_bus.h"

u16 addressing_immediate(cpu* state);
u16 addressing_zeropage(cpu* state);
u16 addressing_zeropagex(cpu* state);
u16 addressing_zeropagey(cpu* state);
u16 addressing_absolute(cpu* state);
u16 addressing_absolutex(cpu* state);
u16 addressing_absolutey(cpu* state);
u16 addressing_indirect(cpu* state);
u16 addressing_indexedindirect(cpu* state);
u16 addressing_indirectindexed(cpu* state);
i8 addressing_relative(cpu* state);

// Access
void opcode_lda(cpu* state, u16 address);
void opcode_sta(cpu* state, u16 address);
void opcode_ldx(cpu* state, u16 address);
void opcode_stx(cpu* state, u16 address);
void opcode_ldy(cpu* state, u16 address);
void opcode_sty(cpu* state, u16 address);

// Transfer
void opcode_tax(cpu* state);
void opcode_tay(cpu* state);
void opcode_txa(cpu* state);

void opcode_tya(cpu* state);

// Arithmetic
void opcode_adc(cpu* state, u16 address);
void opcode_sbc(cpu* state, u16 address);
void opcode_inc(cpu* state, u16 address);
void opcode_dec(cpu* state, u16 address);
void opcode_inx(cpu* state);
void opcode_dex(cpu* state);
void opcode_iny(cpu* state);
void opcode_dey(cpu* state);

// Shift
void opcode_asl(cpu* state, u16 address);
void opcode_asl_accumulator(cpu* state);
void opcode_lsr(cpu* state, u16 address);
void opcode_lsr_accumulator(cpu* state);
void opcode_rol(cpu* state, u16 address);
void opcode_rol_accumulator(cpu* state);
void opcode_ror(cpu* state, u16 address);
void opcode_ror_accumulator(cpu* state);

// Bitwise
void opcode_and(cpu* state, u16 address);
void opcode_ora(cpu* state, u16 address);
void opcode_eor(cpu* state, u16 address);
void opcode_bit(cpu* state, u16 address);

// Compare
void opcode_cmp(cpu* state, u16 address);
void opcode_cpx(cpu* state, u16 address);
void opcode_cpy(cpu* state, u16 address);

// Branch
void opcode_bcc(cpu* state, i8 address);
void opcode_bcs(cpu* state, i8 address);
void opcode_beq(cpu* state, i8 address);
void opcode_bne(cpu* state, i8 address);
void opcode_bpl(cpu* state, i8 address);
void opcode_bmi(cpu* state, i8 address);
void opcode_bvc(cpu* state, i8 address);
void opcode_bvs(cpu* state, i8 address);

// Jump
void opcode_jmp(cpu* state, u16 address);
void opcode_jsr(cpu* state, u16 address);
void opcode_rts(cpu* state);
void opcode_brk(cpu* state);
void opcode_rti(cpu* state);

// Stack
void opcode_pha(cpu* state);
void opcode_pla(cpu* state);
void opcode_php(cpu* state);
void opcode_plp(cpu* state);
void opcode_txs(cpu* state);
void opcode_tsx(cpu* state);

// Flags
void opcode_clc(cpu* state);
void opcode_sec(cpu* state);
void opcode_cli(cpu* state);
void opcode_sei(cpu* state);
void opcode_cld(cpu* state);
void opcode_sed(cpu* state);
void opcode_clv(cpu* state);

// Other
void opcode_nop(cpu* state);

void cpu_init(cpu* state) {
	state->total_cycles = 0;
	state->current_instruction_cycles = 0;
	
	state->program_counter = (cpubus_read(0xFFFD) << 8) | cpubus_read(0xFFFC);
	state->stack_pointer = 0xFD;

	state->accumulator = 0x00;
	state->register_x = 0x00;
	state->register_y = 0x00;

	state->status.carry_flag = 0;
	state->status.zero_flag = 0;
	state->status.interrupt_disable = 1;
	state->status.decimal_flag = 0;
	state->status.overflow_flag = 0;
	state->status.negative_flag = 0;
	state->status.break_flag = 0;
	state->status.unused = 1;

	state->interrupt_flag_changed = 0;
	state->previous_interrupt_flag = 1;
}

void cpu_execute_instruction(cpu* state) {
	u8 instruction = cpubus_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles += 1;

	if (state->interrupt_flag_changed) {
		if (state->previous_interrupt_flag != state->status.interrupt_disable) {
			// The 1 instruction delay for interrupt disable flag
		}
	}

	switch (instruction) {
		//
		// ACCESS
		//

		case 0xA9: opcode_lda(state, addressing_immediate(state)); break;
		case 0xA5: opcode_lda(state, addressing_zeropage(state)); break;
		case 0xB5: opcode_lda(state, addressing_zeropagex(state)); break;
		case 0xAD: opcode_lda(state, addressing_absolute(state)); break;
		case 0xBD: opcode_lda(state, addressing_absolutex(state)); break;
		case 0xB9: opcode_lda(state, addressing_absolutey(state)); break;
		case 0xA1: opcode_lda(state, addressing_indexedindirect(state)); break;
		case 0xB1: opcode_lda(state, addressing_indirectindexed(state)); break;

		case 0x85: opcode_sta(state, addressing_zeropage(state)); break;
		case 0x95: opcode_sta(state, addressing_zeropagex(state)); break;
		case 0x8D: opcode_sta(state, addressing_absolute(state)); break;
		case 0x9D: opcode_sta(state, addressing_absolutex(state)); break;
		case 0x99: opcode_sta(state, addressing_absolutey(state)); break;
		case 0x81: opcode_sta(state, addressing_indexedindirect(state)); break;
		case 0x91: opcode_sta(state, addressing_indirectindexed(state)); break;

		case 0xA2: opcode_ldx(state, addressing_immediate(state)); break;
		case 0xA6: opcode_ldx(state, addressing_zeropage(state)); break;
		case 0xB6: opcode_ldx(state, addressing_zeropagey(state)); break;
		case 0xAE: opcode_ldx(state, addressing_absolute(state)); break;
		case 0xBE: opcode_ldx(state, addressing_absolutey(state)); break;

		case 0x86: opcode_stx(state, addressing_zeropage(state)); break;
		case 0x96: opcode_stx(state, addressing_zeropagey(state)); break;
		case 0x8E: opcode_stx(state, addressing_absolute(state)); break;

		case 0xA0: opcode_ldy(state, addressing_immediate(state)); break;
		case 0xA4: opcode_ldy(state, addressing_zeropage(state)); break;
		case 0xB4: opcode_ldy(state, addressing_zeropagex(state)); break;
		case 0xAC: opcode_ldy(state, addressing_absolute(state)); break;
		case 0xBC: opcode_ldy(state, addressing_absolutex(state)); break;

		case 0x84: opcode_sty(state, addressing_zeropage(state)); break;
		case 0x94: opcode_sty(state, addressing_zeropagex(state)); break;
		case 0x8C: opcode_sty(state, addressing_absolute(state)); break;
		
		//
		// TRANSFER
		//

		case 0xAA: opcode_tax(state); break;
		case 0xA8: opcode_tay(state); break;
		case 0x8A: opcode_txa(state); break;
		case 0x98: opcode_tya(state); break;
		
		//
		// ARITHMETIC
		//

		case 0x69: opcode_adc(state, addressing_immediate(state)); break;
		case 0x65: opcode_adc(state, addressing_zeropage(state)); break;
		case 0x75: opcode_adc(state, addressing_zeropagex(state)); break;
		case 0x6D: opcode_adc(state, addressing_absolute(state)); break;
		case 0x7D: opcode_adc(state, addressing_absolutex(state)); break;
		case 0x79: opcode_adc(state, addressing_absolutey(state)); break;
		case 0x61: opcode_adc(state, addressing_indexedindirect(state)); break;
		case 0x71: opcode_adc(state, addressing_indirectindexed(state)); break;
	
		case 0xE9: opcode_sbc(state, addressing_immediate(state)); break;
		case 0xE5: opcode_sbc(state, addressing_zeropage(state)); break;
		case 0xF5: opcode_sbc(state, addressing_zeropagex(state)); break;
		case 0xED: opcode_sbc(state, addressing_absolute(state)); break;
		case 0xFD: opcode_sbc(state, addressing_absolutex(state)); break;
		case 0xF9: opcode_sbc(state, addressing_absolutey(state)); break;
		case 0xE1: opcode_sbc(state, addressing_indexedindirect(state)); break;
		case 0xF1: opcode_sbc(state, addressing_indirectindexed(state)); break;

		case 0xE6: opcode_inc(state, addressing_zeropage(state)); break;
		case 0xF6: opcode_inc(state, addressing_zeropagex(state)); break;
		case 0xEE: opcode_inc(state, addressing_absolute(state)); break;
		case 0xFE: opcode_inc(state, addressing_absolutex(state)); break;

		case 0xC6: opcode_dec(state, addressing_zeropage(state)); break;
		case 0xD6: opcode_dec(state, addressing_zeropagex(state)); break;
		case 0xCE: opcode_dec(state, addressing_absolute(state)); break;
		case 0xDE: opcode_dec(state, addressing_absolutex(state)); break;

		case 0xE8: opcode_inx(state); break;
		case 0xCA: opcode_dex(state); break;

		case 0xC8: opcode_iny(state); break;
		case 0x88: opcode_dey(state); break;

		//
		// SHIFT
		//

		case 0x0A: opcode_asl_accumulator(state); break;
		case 0x06: opcode_asl(state, addressing_zeropage(state)); break;
		case 0x16: opcode_asl(state, addressing_zeropagex(state)); break;
		case 0x0E: opcode_asl(state, addressing_absolute(state)); break;
		case 0x1E: opcode_asl(state, addressing_absolutex(state)); break;

		case 0x4A: opcode_lsr_accumulator(state); break;
		case 0x46: opcode_lsr(state, addressing_zeropage(state)); break;
		case 0x56: opcode_lsr(state, addressing_zeropagex(state)); break;
		case 0x4E: opcode_lsr(state, addressing_absolute(state)); break;
		case 0x5E: opcode_lsr(state, addressing_absolutex(state)); break;

		case 0x2A: opcode_rol_accumulator(state); break;
		case 0x26: opcode_rol(state, addressing_zeropage(state)); break;
		case 0x36: opcode_rol(state, addressing_zeropagex(state)); break;
		case 0x2E: opcode_rol(state, addressing_absolute(state)); break;
		case 0x3E: opcode_rol(state, addressing_absolutex(state)); break;

		case 0x6A: opcode_ror_accumulator(state); break;
		case 0x66: opcode_ror(state, addressing_zeropage(state)); break;
		case 0x76: opcode_ror(state, addressing_zeropagex(state)); break;
		case 0x6E: opcode_ror(state, addressing_absolute(state)); break;
		case 0x7E: opcode_ror(state, addressing_absolutex(state)); break;

		//
		// BITWISE
		//

		case 0x29: opcode_and(state, addressing_immediate(state)); break;
		case 0x25: opcode_and(state, addressing_zeropage(state)); break;
		case 0x35: opcode_and(state, addressing_zeropagex(state)); break;
		case 0x2D: opcode_and(state, addressing_absolute(state)); break;
		case 0x3D: opcode_and(state, addressing_absolutex(state)); break;
		case 0x39: opcode_and(state, addressing_absolutey(state)); break;
		case 0x21: opcode_and(state, addressing_indexedindirect(state)); break;
		case 0x31: opcode_and(state, addressing_indirectindexed(state)); break;

		case 0x09: opcode_ora(state, addressing_immediate(state)); break;
		case 0x05: opcode_ora(state, addressing_zeropage(state)); break;
		case 0x15: opcode_ora(state, addressing_zeropagex(state)); break;
		case 0x0D: opcode_ora(state, addressing_absolute(state)); break;
		case 0x1D: opcode_ora(state, addressing_absolutex(state)); break;
		case 0x19: opcode_ora(state, addressing_absolutey(state)); break;
		case 0x01: opcode_ora(state, addressing_indexedindirect(state)); break;
		case 0x11: opcode_ora(state, addressing_indirectindexed(state)); break;

		case 0x49: opcode_eor(state, addressing_immediate(state)); break;
		case 0x45: opcode_eor(state, addressing_zeropage(state)); break;
		case 0x55: opcode_eor(state, addressing_zeropagex(state)); break;
		case 0x4D: opcode_eor(state, addressing_absolute(state)); break;
		case 0x5D: opcode_eor(state, addressing_absolutex(state)); break;
		case 0x59: opcode_eor(state, addressing_absolutey(state)); break;
		case 0x41: opcode_eor(state, addressing_indexedindirect(state)); break;
		case 0x51: opcode_eor(state, addressing_indirectindexed(state)); break;

		case 0x24: opcode_bit(state, addressing_zeropage(state)); break;
		case 0x2c: opcode_bit(state, addressing_absolute(state)); break;

		//
		// COMPARE
		//

		case 0xC9: opcode_cmp(state, addressing_immediate(state)); break;
		case 0xC5: opcode_cmp(state, addressing_zeropage(state)); break;
		case 0xD5: opcode_cmp(state, addressing_zeropagex(state)); break;
		case 0xCD: opcode_cmp(state, addressing_absolute(state)); break;
		case 0xDD: opcode_cmp(state, addressing_absolutex(state)); break;
		case 0xD9: opcode_cmp(state, addressing_absolutey(state)); break;
		case 0xC1: opcode_cmp(state, addressing_indexedindirect(state)); break;
		case 0xD1: opcode_cmp(state, addressing_indirectindexed(state)); break;

		case 0xE0: opcode_cpx(state, addressing_immediate(state)); break;
		case 0xE4: opcode_cpx(state, addressing_zeropage(state)); break;
		case 0xEC: opcode_cpx(state, addressing_absolute(state)); break;

		case 0xC0: opcode_cpy(state, addressing_immediate(state)); break;
		case 0xC4: opcode_cpy(state, addressing_zeropage(state)); break;
		case 0xCC: opcode_cpy(state, addressing_absolute(state)); break;

		//
		// BRANCH
		//

		case 0x90: opcode_bcc(state, addressing_relative(state)); break;
		case 0xB0: opcode_bcs(state, addressing_relative(state)); break;
		case 0xF0: opcode_beq(state, addressing_relative(state)); break;
		case 0xD0: opcode_bne(state, addressing_relative(state)); break;
		case 0x10: opcode_bpl(state, addressing_relative(state)); break;
		case 0x30: opcode_bmi(state, addressing_relative(state)); break;
		case 0x50: opcode_bvc(state, addressing_relative(state)); break;
		case 0x70: opcode_bvs(state, addressing_relative(state)); break;

		//
		// JUMP
		//

		case 0x4C: opcode_jmp(state, addressing_absolute(state)); break;
		case 0x6C: opcode_jmp(state, addressing_indirect(state)); break;

		case 0x20: opcode_jsr(state, addressing_absolute(state)); break;
        case 0x60: opcode_rts(state); break;
        case 0x00: opcode_brk(state); break;
        case 0x40: opcode_rti(state); break;

		//
		// STACK
		//
		case 0x48: opcode_pha(state); break;
		case 0x68: opcode_pla(state); break;
		case 0x08: opcode_php(state); break;
		case 0x28: opcode_plp(state); break;
		case 0x9A: opcode_txs(state); break;
		case 0xBA: opcode_tsx(state); break;

		//
		// FLAGS
		//

		case 0x18: opcode_clc(state); break;
		case 0x38: opcode_sec(state); break;
		case 0x58: opcode_cli(state); break;
		case 0x78: opcode_sei(state); break;
		case 0xD8: opcode_cld(state); break;
		case 0xF8: opcode_sed(state); break;
		case 0xB8: opcode_clv(state); break;

		//
		// OTHER
		//

		case 0xEA: opcode_nop(state); break;
	}

	state->total_cycles += state->current_instruction_cycles;
}

u16 addressing_immediate(cpu* state) {
	return state->program_counter++;
}

u16 addressing_zeropage(cpu* state) {
	u8 address = cpubus_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles += 1;

	return address;
}

u16 addressing_zeropagex(cpu* state) {
	u8 address = cpubus_read(state->program_counter) + state->register_x;
	state->program_counter++;
	state->current_instruction_cycles += 2;

	return address;
}

u16 addressing_zeropagey(cpu* state) {
	u8 address = cpubus_read(state->program_counter) + state->register_y;
	state->program_counter++;
	state->current_instruction_cycles += 2;

	return address;
}

u16 addressing_absolute(cpu* state) {
	u16 address = cpubus_read(state->program_counter) | (cpubus_read(state->program_counter + 1) << 8);
	state->program_counter += 2;
	state->current_instruction_cycles += 2;

	return address;
}

u16 addressing_absolutex(cpu* state) {
	u16 base = cpubus_read(state->program_counter) | (cpubus_read(state->program_counter + 1) << 8);
	u16 address = base + state->register_x;
	state->program_counter += 2;
	state->current_instruction_cycles += 3;

	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles += 1;
	}

	return address;
}

u16 addressing_absolutey(cpu* state) {
	u16 base = cpubus_read(state->program_counter) | (cpubus_read(state->program_counter + 1) << 8);
	u16 address = base + state->register_y;
	state->program_counter += 2;
	state->current_instruction_cycles += 3;

	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles += 1;
	}

	return address;
}

u16 addressing_indirect(cpu* state) {
	u16 pointer = cpubus_read(state->program_counter) | (cpubus_read(state->program_counter + 1) << 8);

	u8 low = cpubus_read(pointer);
	u8 high;

	// Bug where if low byte on page boundary then high byte wraps around to page start
	if ((pointer & 0x00FF) == 0x00FF) {
		high = cpubus_read(pointer & 0xFF00);
	}
	else {
		high = cpubus_read(pointer + 1);
	}

	return (high << 8) | low;
}

u16 addressing_indexedindirect(cpu* state) {
	u8 pointer = cpubus_read(state->program_counter) + state->register_x;
	state->program_counter++;

	u16 address = (u16)cpubus_read(pointer) | (u16)(cpubus_read((pointer + 1) & 0xFF) << 8);
	state->current_instruction_cycles += 4;

	return address;
}

u16 addressing_indirectindexed(cpu* state) {
	u8 pointer = cpubus_read(state->program_counter);
	state->program_counter++;

	u16 base = (u16)cpubus_read(pointer) | (u16)(cpubus_read((pointer + 1) & 0xFF) << 8);
	u16 address = base + state->register_y;

	state->current_instruction_cycles += 4;
	if ((base & 0xFF00) != (address & 0xFF00)) {
		state->current_instruction_cycles += 1;
	}

	return address;
}

i8 addressing_relative(cpu* state) {
	i8 value = (i8)cpubus_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles += 1;

	return value;
}

//
// ACCESS
//

void opcode_lda(cpu* state, u16 address) {
	state->accumulator = cpubus_read(address);
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = (state->accumulator & (1 << 7)) != 0;
}

void opcode_sta(cpu* state, u16 address) {
	cpubus_write(address, state->accumulator);
	state->current_instruction_cycles += 1;
}

void opcode_ldx(cpu* state, u16 address) {
	state->register_x = cpubus_read(address);
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = (state->register_x & (1 << 7)) != 0;
}

void opcode_stx(cpu* state, u16 address) {
	cpubus_write(address, state->register_x);
	state->current_instruction_cycles += 1;
}

void opcode_ldy(cpu* state, u16 address) {
	state->register_y = cpubus_read(address);
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = (state->register_x & (1 << 7)) != 0;
}

void opcode_sty(cpu* state, u16 address) {
	cpubus_write(address, state->register_y);
	state->current_instruction_cycles += 1;
}

//
// TRANSFER
//

void opcode_tax(cpu* state) {
	state->register_x = state->accumulator;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = (state->register_x & (1 << 7)) != 0;
}

void opcode_tay(cpu* state) {
	state->register_y = state->accumulator;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = (state->register_x & (1 << 7)) != 0;
}

void opcode_txa(cpu* state) {
	state->accumulator = state->register_x;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = (state->accumulator & (1 << 7)) != 0;
}

void opcode_tya(cpu* state) {
	state->accumulator = state->register_y;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = (state->accumulator & (1 << 7)) != 0;
}

//
// ARITHMETIC
//

void opcode_adc(cpu* state, u16 address) {
	u8 memory = cpubus_read(address);
	u16 result = state->accumulator + memory + state->status.carry_flag;
	state->current_instruction_cycles += 1;

	state->status.carry_flag = result > 0x00FF;
	state->status.zero_flag = (u8)result == 0x00;
	state->status.overflow_flag = (((~(state->accumulator ^ memory)) & (state->accumulator ^ (u8)result)) & 0x80) == 0x80;
	state->status.negative_flag = (result & (1 << 7)) != 0;

	state->accumulator = result & 0xFF;
}

void opcode_sbc(cpu* state, u16 address) {
	u8 memory = cpubus_read(address);
	u16 result = state->accumulator - memory - (1 - state->status.carry_flag);
	state->current_instruction_cycles += 1;

	state->status.carry_flag = result < 0x0100;
	state->status.zero_flag = (u8)result == 0x00;
	state->status.overflow_flag = ((state->accumulator ^ memory) & (state->accumulator ^ (u8)(result & 0xFF)) & 0x80) != 0;
	state->status.negative_flag = (result & (1 << 7)) != 0;

	state->accumulator = result & 0xFF;
}

void opcode_inc(cpu* state, u16 address) {
	u8 value = cpubus_read(address);
	u8 result = value + 1;

	cpubus_write(address, value);
	cpubus_write(address, result);
	state->current_instruction_cycles += 3;

	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;
}

void opcode_dec(cpu* state, u16 address) {
	u8 value = cpubus_read(address);
	u8 result = value - 1;

	cpubus_write(address, value);
	cpubus_write(address, result);
	state->current_instruction_cycles += 3;

	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;
}

void opcode_inx(cpu* state) {
	state->register_x += 1;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = (state->register_x & (1 << 7)) != 0;
}

void opcode_dex(cpu* state) {
	state->register_x -= 1;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = (state->register_x & (1 << 7)) != 0;
}

void opcode_iny(cpu* state) {
	state->register_y += 1;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = (state->register_y & (1 << 7)) != 0;
}

void opcode_dey(cpu* state) {
	state->register_y -= 1;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_y;
	state->status.negative_flag = (state->register_y & (1 << 7)) != 0;
}

//
// SHIFT
//

void opcode_asl(cpu* state, u16 address) {
	u8 value = cpubus_read(address);
	u8 result = value << 1;

	cpubus_write(address, value);
	cpubus_write(address, result);

	state->status.carry_flag = (value & (1 << 7)) == (1 << 7);
	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;

	state->current_instruction_cycles += 3;
}

void opcode_asl_accumulator(cpu* state) {
	state->status.carry_flag = (state->accumulator & (1 << 7)) == (1 << 7);
	state->accumulator = state->accumulator << 1;
	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = (state->accumulator & (1 << 7)) != 0;

	state->current_instruction_cycles += 1;
}

void opcode_lsr(cpu* state, u16 address) {
	u8 value = cpubus_read(address);
	u8 result = value >> 1;

	cpubus_write(address, value);
	cpubus_write(address, result);

	state->status.carry_flag = (value & 1) == 1;
	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;

	state->current_instruction_cycles += 3;
}

void opcode_lsr_accumulator(cpu* state) {
	u8 value = state->accumulator;
	u8 result = value >> 1;

	state->accumulator = result;

	state->status.carry_flag = value & 1;
	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;

	state->current_instruction_cycles += 3;
}

void opcode_rol(cpu* state, u16 address) {
	u8 value = cpubus_read(address);
	u8 result = (value << 1) | state->status.carry_flag;

	cpubus_write(address, value);
	cpubus_write(address, result);

	state->status.carry_flag = (value & (1 << 7)) == (1 << 7);
	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;

	state->current_instruction_cycles += 3;
}

void opcode_rol_accumulator(cpu* state) {
	u8 value = state->accumulator;
	u8 result = (value << 1) | state->status.carry_flag;

	state->accumulator = result;

	state->status.carry_flag = (value & (1 << 7)) == (1 << 7);
	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;
}

void opcode_ror(cpu* state, u16 address) {
	u8 value = cpubus_read(address);
	u8 result = (value >> 1) | (state->status.carry_flag << 7);

	cpubus_write(address, value);
	cpubus_write(address, result);

	state->status.carry_flag = (value & 1) != 0;
	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;

}

void opcode_ror_accumulator(cpu* state) {
	u8 value = state->accumulator;
	u8 result = (value >> 1) | (state->status.carry_flag << 7);

	state->accumulator = result;

	state->status.carry_flag = value & 1;
	state->status.zero_flag = !result;
	state->status.negative_flag = (result & (1 << 7)) != 0;
}

//
// BITWISE
//

void opcode_and(cpu* state, u16 address) {
	state->accumulator = state->accumulator & cpubus_read(address);

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = (state->accumulator & (1 << 7)) != 0;

	state->current_instruction_cycles += 1;
}

void opcode_ora(cpu* state, u16 address) {
	state->accumulator = state->accumulator | cpubus_read(address);

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = (state->accumulator & (1 << 7)) != 0;

	state->current_instruction_cycles += 1;
}

void opcode_eor(cpu* state, u16 address) {
	state->accumulator = state->accumulator ^ cpubus_read(address);

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = (state->accumulator & (1 << 7)) != 0;

	state->current_instruction_cycles += 1;
}

void opcode_bit(cpu* state, u16 address) {
	u8 value = cpubus_read(address);

	state->status.zero_flag = !(state->accumulator & value);
	state->status.negative_flag = (value & (1 << 7)) != 0;
	state->status.overflow_flag = (value & (1 << 6)) == (1 << 6);

	state->current_instruction_cycles += 1;
}

//
// COMPARE
//

void opcode_cmp(cpu* state, u16 address) {
	u8 value = cpubus_read(address);

	state->status.carry_flag = state->accumulator >= value;
	state->status.zero_flag = state->accumulator == value;
	state->status.negative_flag = ((state->accumulator - value) & (1 << 7)) != 0;

	state->current_instruction_cycles += 1;
}

void opcode_cpx(cpu* state, u16 address) {
	u8 value = cpubus_read(address);

	state->status.carry_flag = state->register_x >= value;
	state->status.zero_flag = state->register_x == value;
	state->status.negative_flag = ((state->register_x - value) & (1 << 7)) != 0;

	state->current_instruction_cycles += 1;
}

void opcode_cpy(cpu* state, u16 address) {
	u8 value = cpubus_read(address);

	state->status.carry_flag = state->register_y >= value;
	state->status.zero_flag = state->register_y == value;
	state->status.negative_flag = ((state->register_y - value) & (1 << 7)) != 0;

	state->current_instruction_cycles += 1;
}

//
// BRANCH
//

void opcode_bcc(cpu* state, i8 address) {
	if (!state->status.carry_flag) {
		u16 base = state->program_counter;
		u16 target = base + address;
		
		state->program_counter = target;

		state->current_instruction_cycles += 1;
		if ((base & 0x00FF) != (base & 0x00FF)) {
			state->current_instruction_cycles += 1;
		}
	}
}

void opcode_bcs(cpu* state, i8 address) {
	if (state->status.carry_flag) {
		u16 base = state->program_counter;
		u16 target = base + address;
		
		state->program_counter = target;

		state->current_instruction_cycles += 1;
		if ((base & 0x00FF) != (base & 0x00FF)) {
			state->current_instruction_cycles += 1;
		}
	}
}

void opcode_beq(cpu* state, i8 address) {
	if (state->status.zero_flag) {
		u16 base = state->program_counter;
		u16 target = base + address;
		
		state->program_counter = target;

		state->current_instruction_cycles += 1;
		if ((base & 0x00FF) != (base & 0x00FF)) {
			state->current_instruction_cycles += 1;
		}
	}
}

void opcode_bne(cpu* state, i8 address) {
	if (!state->status.zero_flag) {
		u16 base = state->program_counter;
		u16 target = base + address;
		
		state->program_counter = target;

		state->current_instruction_cycles += 1;
		if ((base & 0x00FF) != (base & 0x00FF)) {
			state->current_instruction_cycles += 1;
		}
	}
}

void opcode_bpl(cpu* state, i8 address) {
	if (!state->status.negative_flag) {
		u16 base = state->program_counter;
		u16 target = base + (i16)address;
		
		state->program_counter = target;

		state->current_instruction_cycles += 1;
		if ((base & 0x00FF) != (target & 0x00FF)) {
			state->current_instruction_cycles += 1;
		}
	}
}

void opcode_bmi(cpu* state, i8 address) {
	if (state->status.negative_flag) {
		u16 base = state->program_counter;
		u16 target = base + address;
		
		state->program_counter = target;

		state->current_instruction_cycles += 1;
		if ((base & 0x00FF) != (base & 0x00FF)) {
			state->current_instruction_cycles += 1;
		}
	}
}

void opcode_bvc(cpu* state, i8 address) {
	if (!state->status.overflow_flag) {
		u16 base = state->program_counter;
		u16 target = base + address;
		
		state->program_counter = target;

		state->current_instruction_cycles += 1;
		if ((base & 0x00FF) != (base & 0x00FF)) {
			state->current_instruction_cycles += 1;
		}
	}
}

void opcode_bvs(cpu* state, i8 address) {
	if (state->status.overflow_flag) {
		u16 base = state->program_counter;
		u16 target = base + address;
		
		state->program_counter = target;

		state->current_instruction_cycles += 1;
		if ((base & 0x00FF) != (base & 0x00FF)) {
			state->current_instruction_cycles += 1;
		}
	}
}

//
// JUMP
//

void opcode_jmp(cpu* state, u16 address) {
	state->program_counter = address;
}

void opcode_jsr(cpu* state, u16 address) {
	state->program_counter--;
	cpubus_write(state->stack_pointer + 0x0100, (state->program_counter & 0xFF00) >> 8);
	state->stack_pointer--;
	cpubus_write(state->stack_pointer + 0x0100, state->program_counter & 0x00FF);
	state->stack_pointer--;

	state->program_counter = address;

	state->current_instruction_cycles += 3;
}

void opcode_rts(cpu* state) {
	state->stack_pointer++;
	u8 low = cpubus_read(state->stack_pointer + 0x0100);
	state->stack_pointer++;
	u8 high = cpubus_read(state->stack_pointer + 0x0100);

	u16 address = (high << 8) | low;
	state->program_counter = address + 1;

	state->current_instruction_cycles += 3;
}

void opcode_brk(cpu* state) {
	state->program_counter++;
	cpubus_write(state->stack_pointer + 0x0100, (state->program_counter & 0xFF00) >> 8);
	state->stack_pointer--;
	cpubus_write(state->stack_pointer + 0x0100, state->program_counter & 0x00FF);
	state->stack_pointer--;

	state->status.break_flag = 1;
	cpubus_write(state->stack_pointer + 0x0100, state->status.as_byte);
	state->stack_pointer--;
	state->status.break_flag = 0;

	state->status.interrupt_disable = 1;

	u8 low = cpubus_read(0xFFFE);
	u8 high = cpubus_read(0xFFFF);
	state->program_counter = (high << 8) | low;

	state->current_instruction_cycles += 6;
}

void opcode_rti(cpu* state) {
	state->stack_pointer++;
	state->status.as_byte = cpubus_read(state->stack_pointer + 0x0100);
	state->status.unused = 1;
	state->status.break_flag = 0;

	state->stack_pointer++;
    u8 low = cpubus_read(state->stack_pointer + 0x0100);
    state->stack_pointer++;
    u8 high = cpubus_read(state->stack_pointer + 0x0100);

	state->program_counter = (high << 8) | low;
	state->current_instruction_cycles += 5;
}

//
// STACK
//

void opcode_pha(cpu* state) {
	cpubus_write(state->stack_pointer + 0x0100, state->accumulator);
	state->stack_pointer--;

	state->current_instruction_cycles += 1;
}

void opcode_pla(cpu* state) {
	state->stack_pointer++;
	state->accumulator = cpubus_read(state->stack_pointer + 0x0100);

	state->status.zero_flag = !state->accumulator;
	state->status.negative_flag = (state->accumulator & (1 << 7)) == (1 << 7);

	state->current_instruction_cycles += 1;
}

void opcode_php(cpu* state) {
	state->status.break_flag = 1;
	cpubus_write(state->stack_pointer + 0x0100, state->status.as_byte);
	state->status.break_flag = 0;
	state->stack_pointer--;

	state->current_instruction_cycles += 1;
}

void opcode_plp(cpu* state) {
	state->stack_pointer++;
	state->status.as_byte = cpubus_read(state->stack_pointer + 0x0100);

	state->status.break_flag = 0;
	state->status.unused = 1;

	state->current_instruction_cycles += 4;
}

void opcode_tsx(cpu* state) {
	state->register_x = state->stack_pointer;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->register_x;
	state->status.negative_flag = ((state->register_x) & (1 << 7)) != 0;
}

void opcode_txs(cpu* state) {
	state->stack_pointer = state->register_x;
	state->current_instruction_cycles += 1;

	state->status.zero_flag = !state->stack_pointer;
	state->status.negative_flag = ((state->stack_pointer) & (1 << 7)) != 0;
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
	state->previous_interrupt_flag = state->status.interrupt_disable;
	state->status.interrupt_disable = 0;
	state->interrupt_flag_changed = 1;

	state->current_instruction_cycles += 1;
}

void opcode_sei(cpu* state) {
	state->previous_interrupt_flag = state->status.interrupt_disable;
	state->status.interrupt_disable = 1;
	state->interrupt_flag_changed = 1;

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

//
// OTHER
//

void opcode_nop(cpu* state) {
	state->current_instruction_cycles += 1;
}