#include "../include/audio.h"

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

bool audio_initialize(SDL_AudioSpec *obtained_spec, SDL_AudioDeviceID *audio_device_ID)
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
    *audio_device_ID = SDL_OpenAudioDevice(NULL, 0, &desired_spec, obtained_spec, 0);
    if (!*audio_device_ID)
    {
        fprintf(stderr, "Could not open audio%s\n", SDL_GetError());
        return false;
    }
    SDL_PauseAudioDevice(*audio_device_ID, 0);
    return true;
}

void audio_play(SDL_AudioDeviceID audio_device, double duration)
{
    int phase = 0;
    int sample_rate = 44100;
    int frequency = 440;
    int amplitude = VOLUME;
    int samples = (int)(sample_rate * duration);
    Sint16 buf[samples];
    for (int i = 0; i < samples; i++, phase++)
    {
        buf[i] = (phase / (sample_rate / frequency) % 2) ? amplitude : -amplitude;
    }
    SDL_ClearQueuedAudio(audio_device);
    SDL_QueueAudio(audio_device, buf, sizeof(buf));
}
