#ifndef WINDOW
#define WINDOW

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "memory.h"
#include "constants.h"

bool window_initialize(SDL_Window **window, SDL_Renderer **renderer);

void window_destroy(SDL_Window *window, SDL_Renderer *renderer);

void window_process_input(bool *state);

void window_render(Memory *memory, SDL_Renderer *renderer);

void window_clear(SDL_Renderer *renderer);

void window_setup(Memory *memory, SDL_Window **window, SDL_Renderer **renderer, bool *state);

#endif