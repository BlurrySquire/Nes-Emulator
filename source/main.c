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

void run_cpu_test(char* filename);

int main(int argc, char* argv[]) {
	if (argc > 2) {
		if (strcmp(argv[1], "--single-step-test") == 0) {
			run_cpu_test(argv[2]);
		}
	}

	SDL_SetAppMetadata("Nes-Emulator", "v0.1", "com.rustygrape238.nesemulator");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	config_load();

	SDL_Window* window = SDL_CreateWindow(
		"Nes-Emulator",
		256 * video_scale, 240 * video_scale,
		0
	);

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

void run_cpu_test(char* filename) {
	printf("Running test: %s", filename);
	FILE* file = fopen(filename, "r");
	fseek(file, 0, SEEK_END);
	u64 length = ftell(file);
	rewind(file);

	char* string = malloc(length + 1);
	fread(string, 1, length, file);
	string[length] = '\0';

	cJSON* root = cJSON_Parse(string);
	if (cJSON_IsArray(root)) {
		printf("Starting tests.\n\n");

		u64 size = cJSON_GetArraySize(root);
		for (u64 i = 0; i < size; i++) {
			bool passed = true;
			cJSON* test = cJSON_GetArrayItem(root, i);

			cpu cpu_state;
			cpubus_init();
			cpu_init(&cpu_state);

			cJSON* name_obj = cJSON_GetObjectItemCaseSensitive(test, "name");
			char* name = cJSON_GetStringValue(name_obj);
			printf("Starting test '%s'\n", name);

			cJSON* initial = cJSON_GetObjectItemCaseSensitive(test, "initial");

			cpu_state.program_counter = cJSON_GetObjectItemCaseSensitive(initial, "pc")->valueint;
			cpu_state.stack_pointer = cJSON_GetObjectItemCaseSensitive(initial, "s")->valueint;
			cpu_state.accumulator = cJSON_GetObjectItemCaseSensitive(initial, "a")->valueint;
			cpu_state.register_x = cJSON_GetObjectItemCaseSensitive(initial, "x")->valueint;
			cpu_state.register_y = cJSON_GetObjectItemCaseSensitive(initial, "y")->valueint;
			cpu_state.status.as_byte = cJSON_GetObjectItemCaseSensitive(initial, "p")->valueint;

			cJSON* ram = cJSON_GetObjectItemCaseSensitive(initial, "ram");
			if (cJSON_IsArray(ram)) {
				u64 size = cJSON_GetArraySize(ram);
				for (u64 i = 0; i < size; i++) {
					cJSON* array = cJSON_GetArrayItem(ram, i);
					cpubus_write(cJSON_GetArrayItem(array, 0)->valueint, cJSON_GetArrayItem(array, 1)->valueint);
				}
			}
			else {
				printf("Error reading 1.\n");
				exit(-1);
			}

			cpu_execute_instruction(&cpu_state);

			cJSON* result = cJSON_GetObjectItemCaseSensitive(test, "final");
			u16 program_counter = cJSON_GetObjectItemCaseSensitive(result, "pc")->valueint;
			u8 stack_pointer = cJSON_GetObjectItemCaseSensitive(result, "s")->valueint;
			u8 accumulator = cJSON_GetObjectItemCaseSensitive(result, "a")->valueint;
			u8 register_x = cJSON_GetObjectItemCaseSensitive(result, "x")->valueint;
			u8 register_y = cJSON_GetObjectItemCaseSensitive(result, "y")->valueint;
			u8 status = cJSON_GetObjectItemCaseSensitive(result, "p")->valueint;

			cJSON* result_ram = cJSON_GetObjectItemCaseSensitive(result, "ram");
			if (cJSON_IsArray(ram)) {
				u64 size = cJSON_GetArraySize(result_ram);
				for (u64 i = 0; i < size; i++) {
					cJSON* array = cJSON_GetArrayItem(result_ram, i);
					u16 address = cJSON_GetArrayItem(array, 0)->valueint;
					u8 should_be = cJSON_GetArrayItem(array, 1)->valueint;
					u8 is = cpubus_read(address);
					if (is != should_be) {
						printf("Ram 0x%04X should be 0x%02X but is 0x%02X\n", address, should_be, is);
						passed = false;
					}
				}
			}
			else {
				printf("Error reading 2.\n");
				exit(-1);
			}

			if (program_counter != cpu_state.program_counter) {
				printf("PC should be 0x%04X but is 0x%04X\n", program_counter, cpu_state.program_counter);
				passed = false;
			}

			if (accumulator != cpu_state.accumulator) {
				printf("A should be 0x%02X but is 0x%02X\n", accumulator, cpu_state.accumulator);
				passed = false;
			}

			if (register_x != cpu_state.register_x) {
				printf("X should be 0x%02X but is 0x%02X\n", register_x, cpu_state.register_x);
				passed = false;
			}

			if (register_y != cpu_state.register_y) {
				printf("Y should be 0x%02X but is 0x%02X\n", register_y, cpu_state.register_y);
				passed = false;
			}

			if (status != cpu_state.status.as_byte) {
				printf("Status should be 0x%02X but is 0x%02X\n", status, cpu_state.status.as_byte);
				passed = false;
			}

			if (passed) {
				printf("Test Passed.\n\n");
			}
			else {
				printf("Test Failed.\n");
				exit(-1);
			}

			cJSON_free(test);
		}
	}
	else {
		printf("Wrong file.\n");
		exit(-1);
	}

	exit(0);
}