/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// modes.hh: includes for mode run/draw calls

#ifndef M_MODES_HH
#define M_MODES_HH

enum class GameMode
{
    None,
    Logo,
    TitleScreen,
    HighScoreScreen,
    NameEntry,
    SelectLevel,
    Cutscene,
    Game,
    Credits,
};

#include "render.hh"

void DrawLogoFrame(Framebuffer &fb);

void RunLogoFrame();

#endif // M_MODES_HH
