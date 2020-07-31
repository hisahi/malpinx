/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// modes.hh: includes for mode run/draw calls

#ifndef M_MODES_HH
#define M_MODES_HH

#include <string>
#include "render.hh"

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

void InitLogo(int seqnum, const std::string &name);
void InitTitleScreen(bool instant);

void DrawLogoFrame(Image &fb);
void DrawTitleFrame(Image &fb);

void RunLogoFrame();
void RunTitleFrame();

#endif // M_MODES_HH
