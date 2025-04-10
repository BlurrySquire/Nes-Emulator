#pragma once

#include "types.h"

struct status {
	u8 carry_flag : 1;
	u8 zero_flag : 1;
	u8 interrupt_disable : 1;
	u8 decimal_flag : 1;
	u8 overflow_flag : 1;
	u8 negative_flag : 1;
	u8 the_b_flag : 1;
};

typedef struct cpu {
	u64 total_cycles;
	u64 current_instruction_cycles;

	u16 program_counter;
	u8 accumulator;
	u8 register_x, register_y;
	struct status register_status;
	u8 stack_pointer;

	u8 interrupt_flag;
	u8 instruction_delay;
} cpu;

void cpu_init(cpu* state);
void cpu_execute_instruction(cpu* state);