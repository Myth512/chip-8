#include "../include/include.h"

int main(int argc, char *argv[]){
	Memory memory;
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	bool state = false;

	setup(&memory, &window, &renderer, &state);	
	rom_read(argc, argv, &memory, &state);

	while(state){
		process_input(&state);

		u16 instruction = instruction_fetch(&memory);
		instruction_decode(instruction);
	}

	window_destroy(window, renderer);

	return 0;
}
