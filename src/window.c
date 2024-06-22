#include "../include/window.h"

bool window_initialize(SDL_Window **window, SDL_Renderer **renderer){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Could not initialize SDL: \n%s", SDL_GetError());
		return false;
	}

	*window = SDL_CreateWindow(
		"CHIP-8 emulator",
		0,
		0,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0	
	);

	if (!*window){
		fprintf(stderr, "Could not create window.\n");
		return false;
	}

	*renderer = SDL_CreateRenderer(*window, -1, 0);
	if (!*renderer){
		fprintf(stderr, "Could not create renderer.\n");
		return false;
	}
	
	return true;
}

void window_destroy(SDL_Window *window, SDL_Renderer *renderer){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return;
}

void window_process_input(bool *state) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			*state = false;
			break;	
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				*state = false;
			break;
	}
	return;
}

void window_render(Memory *memory, SDL_Renderer *renderer){
	SDL_RenderClear(renderer);
	for (u8 y = 0; y < 32; y++){
		for (u8 x = 0; x < 64; x++){
			if (memory->screen[y][x])
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			else
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_Rect pixel = {x * SCALE, y * SCALE, SCALE, SCALE};
			SDL_RenderFillRect(renderer, &pixel);
		}
	}
	SDL_RenderPresent(renderer);
}

void window_clear(SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect = { 0, 0, 64 * SCALE, 32 * SCALE};
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    return;
}

void window_setup(Memory *memory, SDL_Window **window, SDL_Renderer **renderer, bool *state){
	memory_initialize(memory);
	*state = window_initialize(window, renderer);
	if (state)
		window_render(memory, *renderer);
	return;
}
