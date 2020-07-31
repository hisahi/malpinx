/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sdl2.hh: header file for SDL2 base

#ifndef M_SDL2_HH
#define M_SDL2_HH

#include <stdexcept>
#include <sstream>
#include <SDL2/SDL.h>

inline std::string SDL2_build_message(const std::string& message)
{
    std::ostringstream stream;
    stream << message << ": " << SDL_GetError();
    return stream.str();
}

class SDLException : public std::runtime_error
{
public:
    SDLException(const std::string& message)
        : std::runtime_error(SDL2_build_message(message))
    {
    }
};

void ibase_before_events();
void ibase_handle_event(const SDL_Event& evt);

#endif // M_SDL2_HH
