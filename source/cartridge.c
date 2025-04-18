#include "cartridge.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mapper.h"

// iNES ROM Header
typedef struct rom_header {
	char name[4];
	u8 prg_rom_size;
	u8 chr_rom_size;

	union flags6 {
		struct {
			u8 nametable_arrangement : 1;
			u8 persistent_memory : 1;
			u8 trainer : 1;
			u8 alternative_nametable_layout : 1;
			u8 mapper_lower : 4;
		};
		u8 as_byte;
	} flags6;

	union flags7 {
		struct {
			u8 vs_unisystem : 1;
			u8 playchoice_10 : 1;
			u8 nes_2 : 2;
			u8 mapper_upper : 4;
		};
		u8 as_byte;
	} flags7;

	u8 prg_ram_size;
	u8 tv_system;

	u8 unused[6];
} rom_header;

u8 mapper;
u8* prg_ram = NULL;
u8* prg_rom = NULL;
u8* chr_rom = NULL;
rom_header header;

int cartridge_init(const char* rom_path) {
	FILE* file = fopen(rom_path, "rb");
	if (file == NULL) {
		return -1;
	}

	fread(&header, sizeof(rom_header), 1, file);

	const unsigned char expected[4] = { 'N', 'E', 'S', 0x1A };
	if (memcmp(header.name, expected, 4) != 0) {
		printf("ROM Header is incorrect.\n");
		fclose(file);
		return -1;
	}

	if (header.flags6.trainer == 1) {
		fseek(file, 512, SEEK_CUR);
	}

	mapper = (header.flags6.mapper_lower & 0x0F) | ((header.flags7.mapper_upper & 0xF0));

	if (mapper == 0) {
		prg_rom = malloc(header.prg_rom_size * (16 * 1024));
		chr_rom = malloc(header.chr_rom_size * (8 * 1024));
		prg_ram = malloc(8 * 1024);

		fread(prg_rom, header.prg_rom_size * (16 * 1024), 1, file);
		fread(chr_rom, header.chr_rom_size * (8 * 1024), 1, file);
	}
	else {
		fclose(file);
		return -1;
	}

	fclose(file);
	return 0;
}

u8 cartridge_read(u16 address) {
	if (mapper == 0) {
		return mapper0_read(address, prg_ram, prg_rom, header.prg_rom_size);
	}
	
	return 0x00;
}

void cartridge_write(u16 address, u8 value) {
	if (mapper == 0) {
		mapper0_write(address, value, prg_ram);
	}
}