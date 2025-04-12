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

	cartridge_init("test.nes");

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

		cpu_execute_instruction(&cpu_state);

		#ifndef NDEBUG
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "CPU State:\n");
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
				"PC: 0x%04X, A: 0x%02X, X: 0x%02X, Y: 0x%02X \n\n",
				cpu_state.program_counter, cpu_state.accumulator, cpu_state.register_x, cpu_state.register_y
			);
		#endif
	}

	SDL_Quit();
	return 0;
}