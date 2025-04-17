#pragma once

#include "types.h"

int cartridge_init(char* rom_path);

u8 cartridge_read(u16 address);
void cartridge_write(u16 address, u8 value);