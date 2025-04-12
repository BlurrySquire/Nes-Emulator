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
	cpu_init(&cpu_state);
	
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