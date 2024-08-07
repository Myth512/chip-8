#include "../include/window.h"

const int key_bindings[] = {
	SDL_SCANCODE_X, // 0
	SDL_SCANCODE_1, // 1
	SDL_SCANCODE_2, // 2
	SDL_SCANCODE_3, // 3
	SDL_SCANCODE_Q, // 4
	SDL_SCANCODE_W, // 5
	SDL_SCANCODE_E, // 6
	SDL_SCANCODE_A, // 7
	SDL_SCANCODE_S, // 8
	SDL_SCANCODE_D, // 9
	SDL_SCANCODE_Z, // A
	SDL_SCANCODE_C, // B
	SDL_SCANCODE_4, // C
	SDL_SCANCODE_R, // D
	SDL_SCANCODE_F, // E
	SDL_SCANCODE_V	// F
};

bool window_initialize(SDL_Window **window, SDL_Renderer **renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Could not initialize SDL: \n%s", SDL_GetError());
		return false;
	}

	*window = SDL_CreateWindow(
		"CHIP-8 emulator",
		0,
		0,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0);

	if (!*window)
	{
		fprintf(stderr, "Could not create window.\n");
		return false;
	}

	*renderer = SDL_CreateRenderer(*window, -1, 0);
	if (!*renderer)
	{
		fprintf(stderr, "Could not create renderer.\n");
		return false;
	}

	return true;
}

void window_destroy(SDL_Window *window, SDL_Renderer *renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return;
}

void window_process_input(bool *state)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
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

void window_draw_sprite(Memory *memory, SDL_Renderer *renderer, u8 origin_x, u8 origin_y, u8 height)
{
	for (u8 y = 0; y < height; y++)
	{
		for (u8 x = 0; x < 8; x++)
		{
			u8 real_x = (origin_x + x) & 127;
			u8 real_y = (origin_y + y) & 63;

			if (memory->screen[real_y][real_x])
				SDL_SetRenderDrawColor(renderer, PIXEL_ON_RED, PIXEL_ON_GREEN, PIXEL_ON_BLUE, 255);
			else
				SDL_SetRenderDrawColor(renderer, PIXEL_OFF_RED, PIXEL_OFF_GREEN, PIXEL_OFF_BLUE, 255);

			SDL_Rect pixel = {real_x * SCALE, real_y * SCALE, SCALE, SCALE};
			SDL_RenderFillRect(renderer, &pixel);
		}
	}
	SDL_RenderPresent(renderer);
	return;
}

void window_render(Memory *memory, SDL_Renderer *renderer)
{
	for (u8 y = 0; y < 64; y++)
	{
		for (u8 x = 0; x < 128; x++)
		{
			if (memory->screen[y][x])
				SDL_SetRenderDrawColor(renderer, PIXEL_ON_RED, PIXEL_ON_GREEN, PIXEL_ON_BLUE, 255);
			else
				SDL_SetRenderDrawColor(renderer, PIXEL_OFF_RED, PIXEL_OFF_GREEN, PIXEL_OFF_BLUE, 255);

			SDL_Rect pixel = {x * SCALE, y * SCALE, SCALE, SCALE};
			SDL_RenderFillRect(renderer, &pixel);
		}
	}
	SDL_RenderPresent(renderer);
	return;
}

void window_clear(SDL_Renderer *renderer)
{
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, PIXEL_OFF_RED, PIXEL_OFF_GREEN, PIXEL_OFF_BLUE, 255);
	SDL_Rect rect = {0, 0, 128 * SCALE, 64 * SCALE};
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
	return;
}
