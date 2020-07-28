/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sdl2/abase.cc: implementation of base audio library on SDL2

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include "base/sdl2.hh"

void abase_init()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);
}

void abase_quit()
{
}
