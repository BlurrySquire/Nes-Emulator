#pragma once

#include "types.h"
#include "cpu.h"

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