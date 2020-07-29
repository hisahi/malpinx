/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sdlmixer/abase.cc: implementation of base audio library on SDL2 SDL_mixer

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#include "base/sdl2.hh"

using std::cerr;
using std::endl;

void abase_init(int sampleRate)
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        cerr << "Could not initialize SDL_audio; no music/sound" << endl;
        return;
    }
    if (Mix_OpenAudio(sampleRate, MIX_DEFAULT_FORMAT, 2, 1024))
    {
        cerr << "Could not initialize SDL_mixer; no music/sound" << endl;
        return;
    }
}

void abase_quit()
{
    Mix_CloseAudio();
}
