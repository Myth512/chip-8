#include "../include/include.h"

bool initialize_all(Memory *memory, SDL_Window **window, SDL_Renderer **renderer, SDL_AudioSpec *audio_spec)
{
	memory_initialize(memory);
	if (!window_initialize(window, renderer))
        return false;
    if (!sound_initialize(audio_spec))
        return false;
	return true;
}

int main(int argc, char *argv[])
{
    Memory memory;
    SDL_AudioSpec audio_spec;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    bool state = false;
    state = initialize_all(&memory, &window, &renderer, &audio_spec);
    state = memory_rom_read(argc, argv, &memory);

    while(state)
    {
        window_process_input(&state);
        instruction_execute(&memory, renderer, &state); 
        instruction_update_timers(&memory); 
    }
    window_destroy(window, renderer);
    SDL_CloseAudio();
    return 0;
}
