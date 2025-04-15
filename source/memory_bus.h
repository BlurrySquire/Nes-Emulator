#pragma once

#include "types.h"

void cpubus_init();
void cpubus_enable_testmode(u8* memory);
u8 cpubus_read(u16 address);
void cpubus_write(u16 address, u8 value);

void ppubus_init();
u8 ppubus_read(u16 address);
void ppubus_write(u16 address, u8 value);