#pragma once

#include "types.h"

union status {
	struct {
		u8 carry_flag : 1;
		u8 zero_flag : 1;
		u8 interrupt_disable : 1;
		u8 decimal_flag : 1;
		u8 break_flag : 1;
		u8 unused : 1;
		u8 overflow_flag : 1;
		u8 negative_flag : 1;
	};
	u8 as_byte;
};

typedef struct cpu {
	u64 total_cycles;
	u64 current_instruction_cycles;

	u16 program_counter;
	u8 accumulator;
	u8 register_x, register_y;
	union status status;
	u8 stack_pointer;

	u8 interrupt_flag_changed;
	u8 previous_interrupt_flag;
} cpu;

void cpu_init(cpu* state);
void cpu_execute_instruction(cpu* state);