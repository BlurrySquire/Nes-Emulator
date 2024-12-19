#include <SDL.h>
#include <SDL_main.h>

#include "memory_bus.h"

int main(int argc, char* argv[]) {	
	memory_init();

	memory_write(0x0000, 0x10);
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Memory at 0x0000: %i", memory_read(0x0000));

	return 0;
}