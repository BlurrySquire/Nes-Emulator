#pragma once

#include "types.h"

void memory_init();

void memory_write(u16 location, u8 data);
u8 memory_read(u16 location);