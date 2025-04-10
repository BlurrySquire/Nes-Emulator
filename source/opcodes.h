#pragma once

#include "types.h"
#include "cpu.h"

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

// Jump
void opcode_jmp(cpu* state, u16 address);

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