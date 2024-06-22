#ifndef WINDOW
#define WINDOW

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "memory.h"
#include "constants.h"

bool window_initialize(SDL_Window **window, SDL_Renderer **renderer);

void window_destroy(SDL_Window *window, SDL_Renderer *renderer);

void process_input(bool *state);

void render(Memory *memory, SDL_Renderer *renderer);

void setup(Memory *memory, SDL_Window **window, SDL_Renderer **renderer, bool *state);

#endif
