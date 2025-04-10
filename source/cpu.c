#include "cpu.h"

#include "memory_bus.h"
#include "addressing_modes.h"
#include "opcodes.h"

void cpu_init(cpu* state) {
	state->total_cycles = 0;
	state->current_instruction_cycles = 0;
	
	state->program_counter = 0x0000;
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

	state->interrupt_flag = 0;
}

void cpu_execute_instruction(cpu* state) {
	u8 instruction = memory_read(state->program_counter);
	state->program_counter++;
	state->current_instruction_cycles += 1;

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
		case 0x5E: opcode_lsr(state, addressing_absolutex(state));

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

	// CLI & SEI have a 1 instruction delay.
	if (state->instruction_delay == 0xFF) {
		if (state->interrupt_flag == 0) {
			state->status.interrupt_disable = 0;
		}
		else if (state->interrupt_flag == 1) {
			state->status.interrupt_disable = 1;
		}

		state->instruction_delay = 0;
	}
	else if (state->instruction_delay == 1) {
		state->instruction_delay = 0xFF;
	}

	state->total_cycles += state->current_instruction_cycles;
}