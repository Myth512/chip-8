#include "../include/include.h"

int main(int argc, char *argv[])
{
    Memory memory;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    bool state = false;
    time_t start, end;
    window_setup(&memory, &window, &renderer, &state);	
    memory_rom_read(argc, argv, &memory, &state);

    while(state)
    {
        start = clock();
        window_process_input(&state);

        u16 instruction = instruction_fetch(&memory);
        instruction_decode(instruction, &memory, renderer, &state);
        instruction_update_timers(&memory);

        end = clock();
        window_sleep(start, end);
    }
    window_destroy(window, renderer);
    return 0;
}
