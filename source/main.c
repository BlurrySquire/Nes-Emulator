#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cjson/cJSON.h>

#include <stdio.h>
#include <stdlib.h>

#include "memory_bus.h"
#include "cartridge.h"
#include "cpu.h"

int video_scale = 1;

void config_load();
void config_reset();

int main(int argc, char* argv[]) {
	SDL_SetAppMetadata("Nes-Emulator", "v0.1", "com.rustygrape238.nesemulator");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	config_load();

	SDL_Window* window = SDL_CreateWindow(
		"Nes-Emulator",
		256 * video_scale, 240 * video_scale,
		SDL_WINDOW_HIDDEN
	);
	SDL_ShowWindow(window);

	if (window == NULL) {
		SDL_Quit();
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL2: Failed to open a window");
		return -1;
	}

	cpubus_init();
	cartridge_init("test.nes");
	
	cpubus_write(0xFFFC, 0x00);
	cpubus_write(0xFFFD, 0xF0);

	cpubus_write(0xF000, 0xA2); // LDA Immediate 0xFF
	cpubus_write(0xF001, 0xFF);
	cpubus_write(0xF002, 0x9A); // TXS
	cpubus_write(0xF003, 0xEA); // NOP
	cpubus_write(0xF004, 0x4C); // JMP Absolute 0xF003
	cpubus_write(0xF005, 0x03);
	cpubus_write(0xF006, 0xF0);

	cpu cpu_state;
	cpu_init(&cpu_state);
	
	bool running = true;
	while (running == true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		#ifndef NDEBUG
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "CPU State:\n");
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
				"PC: 0x%04X, SP: 0x%02X\n",
				cpu_state.program_counter, cpu_state.stack_pointer
			);
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
				"A: 0x%02X, X: 0x%02X, Y: 0x%02X\n",
				cpu_state.accumulator, cpu_state.register_x, cpu_state.register_y
			);
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
				"N: %i, V: %i, B: %i, D: %i, I: %i, Z: %i, C: %i\n\n",
				cpu_state.status.negative_flag, cpu_state.status.overflow_flag, cpu_state.status.break_flag,
				cpu_state.status.decimal_flag, cpu_state.status.interrupt_disable, cpu_state.status.zero_flag,
				cpu_state.status.carry_flag
			);
		#endif

		cpu_execute_instruction(&cpu_state);
	}

	SDL_Quit();
	return 0;
}

void config_load() {
	FILE* file = fopen("config.json", "r");
	if (file == NULL) {
		config_reset();
	}
	else {
		fseek(file, 0, SEEK_END);
		u64 length = ftell(file);
		rewind(file);

		char* contents = malloc(length + 1);
		fread(contents, 1, length, file);
		contents[length] = '\0';

		cJSON* root = cJSON_Parse(contents);
		cJSON* video_scale_obj = cJSON_GetObjectItemCaseSensitive(root, "video_scale");
		if (cJSON_IsNumber(video_scale_obj)) {
			video_scale = video_scale_obj->valueint;
		}
		else {
			config_reset();
		}

		fclose(file);
	}
}

void config_reset() {
	cJSON* root = cJSON_CreateObject();
	if (!root) return;

	cJSON_AddNumberToObject(root, "video_scale", 1);

	char* json_string = cJSON_Print(root);

	FILE* file = fopen("config.json", "w");
	if (file) {
		fputs(json_string, file);
		fclose(file);
	}

	cJSON_Delete(root);
}