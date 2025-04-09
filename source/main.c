#include <SDL.h>
#include <SDL_main.h>

#include "memory_bus.h"
#include "cpu.h"

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"Nes-Emulator",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		256, 240,
		SDL_WINDOW_SHOWN
	);

	if (window == NULL) {
		SDL_Quit();
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL2: Failed to open a window");
		return -1;
	}

	cpu cpu_state;
	memory_init();
	cpu_init(&cpu_state);

	memory_write(0x0000, 0x4C); // JMP $0100
	memory_write(0x0001, 0x00);
	memory_write(0x0002, 0x01);
	
	memory_write(0x0100, 0xA2); // LDX #$10
	memory_write(0x0101, 0x10);
	
	memory_write(0x0102, 0x86); // STX $03
	memory_write(0x0103, 0x03);
	
	memory_write(0x0104, 0xA4); // LDY $03
	memory_write(0x0105, 0x03);
	
	memory_write(0x0106, 0x84); // STY $04
	memory_write(0x0107, 0x04);
	
	memory_write(0x0108, 0xA5); // LDA $04
	memory_write(0x0109, 0x04);
	
	memory_write(0x010A, 0x00); // BRK
	
	SDL_bool running = SDL_TRUE;
	while (running == SDL_TRUE) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = SDL_FALSE;
			}
		}

		cpu_execute_instruction(&cpu_state);
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Program Counter: %i", cpu_state.program_counter);
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Accumulator: %i", cpu_state.accumulator);
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Register X: %i", cpu_state.register_x);
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Register Y: %i", cpu_state.register_y);

	SDL_Quit();
	return 0;
}