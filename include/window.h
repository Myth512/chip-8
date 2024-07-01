#ifndef WINDOW
#define WINDOW

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

#include "memory.h"
#include "constants.h"

extern const int key_bindings[16];

bool window_initialize(SDL_Window **window, SDL_Renderer **renderer);

void window_destroy(SDL_Window *window, SDL_Renderer *renderer);

void window_process_input(bool *state);

void window_draw_sprite(Memory *memory, SDL_Renderer *renderer, u8 x, u8 y, u8 height);

void window_clear(SDL_Renderer *renderer);

void window_render(Memory *memory, SDL_Renderer *renderer, clock_t *last_render);

#endif
