#include "../include/sound.h"

void audio_callback(void *userdata, Uint8 *stream, int len)
{
    static int phase = 0;
    int sample_rate = 44100;
    int frequency = 440;
    int amplitude = 20 * VOLUME;
    int samples = len / 2;
    Sint16 *buf = (Sint16 *)stream;

    for (int i = 0; i < samples; i++, phase++)
    {
        buf[i] = (phase / (sample_rate / frequency) % 2) ? amplitude : -amplitude;
    }
}

bool sound_initialize(SDL_AudioSpec *obtained_spec)
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "Could not initialize audio%s\n", SDL_GetError());
        return false;
    }
    
    SDL_AudioSpec desired_spec;
    SDL_zero(desired_spec);
    desired_spec.freq = 44100;
    desired_spec.format = AUDIO_S16SYS;
    desired_spec.channels = 1;
    desired_spec.samples = 4096;
    desired_spec.callback = audio_callback;

    if (SDL_OpenAudio(&desired_spec, obtained_spec) < 0)
    {
        fprintf(stderr, "Could not open audio%s\n", SDL_GetError());
        return false;
    }
    return true;
}