#include "../include/include.h"

int main(int argc, char *argv[]) {
    Memory memory;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    bool state = false;

    window_setup(&memory, &window, &renderer, &state);	
    memory_rom_read(argc, argv, &memory, &state);

    while(state){
        window_process_input(&state);

        u16 instruction = instruction_fetch(&memory);
        instruction_decode(instruction, &memory, renderer, &state);
    }

    window_destroy(window, renderer);
    return 0;
}
