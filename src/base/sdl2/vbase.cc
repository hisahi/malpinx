/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sdl2/vbase.cc: implementation of base graphics library on SDL2

#include <SDL2/SDL.h>
#include "defs.hh"
#include "render.hh"
#include "base/sdl2.hh"
#include "main.hh"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Texture *screen;
static bool quit = false;
static Uint32 palette[0x1000];
static unsigned int ticks = 0;
static unsigned long long frac = 0ULL;

int SDL_main(int argc, char **argv)
{
    return MalpinxMain(argc, argv);
}

static inline int extend_color_channel(int v)
{
    return (v << 4) | v;
}

void sdl_build_palette()
{
    Color clr;
    for (int i = 0; i < 0x1000; ++i)
    {
        clr = Color(i);
        palette[i] = SDL_MapRGB(surface->format,
            extend_color_channel(clr.getR()),
            extend_color_channel(clr.getG()),
            extend_color_channel(clr.getB()));
    }
}

void vbase_init()
{
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO))
        throw SDLException("Could not initialize SDL2");
    if (!(window = SDL_CreateWindow("MALPINX", 50, 50,
                    S_WIDTH, S_HEIGHT, SDL_WINDOW_SHOWN)))
        throw SDLException("Could not initialize SDL2 window");
    if (!(renderer = SDL_CreateRenderer(window, -1,
                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
        throw SDLException("Could not initialize SDL2 renderer");
    if (!(surface = SDL_CreateRGBSurface(0, S_WIDTH, S_HEIGHT, 32,
                    0, 0, 0, 0)))
        throw SDLException("Could not initialize SDL2 surface");
    if (!(screen = SDL_CreateTextureFromSurface(renderer, surface)))
        throw SDLException("Could not initialize SDL2 texture");
    ticks = SDL_GetTicks();
    sdl_build_palette();
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
}

void vbase_set_scale(int scale)
{
    if (scale > 0)
        SDL_SetWindowSize(window, S_WIDTH * scale, S_HEIGHT * scale);
}

// should game stay running?
bool vbase_loop()
{
    if (quit) return false;
    ibase_before_events();
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            ibase_handle_event(event);
            break;
        }
    }
    return !quit;
}

// renders screen
void vbase_flip()
{
    SDL_LockSurface(surface);
    int stride = surface->pitch / sizeof(Uint32);
    Color *src = fb_front.buffer().data();
    Uint32 *dst = static_cast<Uint32 *>(surface->pixels);
    for (int y = 0; y < S_HEIGHT; ++y)
        for (int x = 0; x < S_WIDTH; ++x)
            dst[y * stride + x] = palette[src[y * S_STRIDE + x].v & 0x0FFF];
    SDL_UpdateTexture(screen, NULL, surface->pixels, surface->pitch);
    SDL_UnlockSurface(surface);
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// waits for tick to pass
void vbase_sync()
{
    static unsigned int new_ticks;
    while (frac < S_TICK_US)
    {
        new_ticks = SDL_GetTicks();
        frac += 1000ULL * (new_ticks - ticks);
        ticks = new_ticks;
        SDL_Delay(1);
    }

    frac %= S_TICK_US;
}

void vbase_quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
