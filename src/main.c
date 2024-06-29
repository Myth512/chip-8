#include "../include/include.h"

int main(int argc, char *argv[])
{
    Memory memory;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    bool state = false;
    clock_t last_instruction_executed = 0, last_timer_update = 0;
    window_setup(&memory, &window, &renderer, &state);	
    memory_rom_read(argc, argv, &memory, &state);

    while(state)
    {
        window_process_input(&state);
        instruction_execute(&memory, renderer, &state, &last_instruction_executed);
        instruction_update_timers(&memory, &last_timer_update);
    }
    window_destroy(window, renderer);
    return 0;
}
