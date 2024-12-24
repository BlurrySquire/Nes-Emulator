#include <SDL.h>
#include <SDL_main.h>

#include "memory_bus.h"
#include "cpu.h"

int main(int argc, char* argv[]) {	
	cpu cpu_state;
	
	memory_init();
	cpu_init(&cpu_state);

	memory_write(0x0000, 0xA9); // LDA - #Immediate
	memory_write(0x0001, 0x10);
	memory_write(0x0002, 0xA2); // LDX - #Immediate
	memory_write(0x0003, 0x10);
	memory_write(0x0004, 0xA0); // LDY - #Immediate
	memory_write(0x0005, 0x10);
	memory_write(0x0006, 0xEA); // NOP - Implied

	cpu_execute_instruction(&cpu_state);
	cpu_execute_instruction(&cpu_state);
	cpu_execute_instruction(&cpu_state);
	cpu_execute_instruction(&cpu_state);

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Accumulator: %i", cpu_state.accumulator);
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Register X: %i", cpu_state.register_x);
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Register Y: %i", cpu_state.register_y);

	return 0;
}