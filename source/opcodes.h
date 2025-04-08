#pragma once

#include "types.h"
#include "cpu.h"

void opcode_nop(cpu* state);

void opcode_lda(cpu* state, u16 address);
void opcode_ldx(cpu* state, u16 address);
void opcode_ldy(cpu* state, u16 address);

void opcode_jmp(cpu* state, u16 address);