#pragma once

#include "types.h"

u8 mapper0_read(u16 address, u8* prg_ram, u8* prg_rom, u8 prg_rom_size);
void mapper0_write(u16 address, u8 value, u8* prg_ram);