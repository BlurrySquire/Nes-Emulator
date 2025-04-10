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
		// JUMP
		//

		case 0x4C: opcode_jmp(state, addressing_absolute(state)); break;
		case 0x6C: opcode_jmp(state, addressing_indirect(state)); break;

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