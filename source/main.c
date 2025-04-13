#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "memory_bus.h"
#include "cartridge.h"
#include "cpu.h"

int main(int argc, char* argv[]) {
	SDL_SetAppMetadata("Nes-Emulator", "v0.1", "com.rustygrape238.nesemulator");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	SDL_Window* window = SDL_CreateWindow(
		"Nes-Emulator",
		256, 240,
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