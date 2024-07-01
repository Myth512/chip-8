#ifndef SOUND
#define SOUND

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "constants.h"

void audio_callback(void *userdata, Uint8 *stream, int len);

bool sound_initialize(SDL_AudioSpec *audio_spec); 

#endif