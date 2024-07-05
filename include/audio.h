#ifndef SOUND
#define SOUND

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "constants.h"

void audio_callback(void *userdata, Uint8 *stream, int len);

bool audio_initialize(SDL_AudioSpec *obtained_spec, SDL_AudioDeviceID *audio_device_ID);

void audio_play(SDL_AudioDeviceID audio_device, double duration);

#endif