/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// render.cc: top-level class for frame drawing

#include <algorithm>
#include <iterator>
#include "render.hh"
#include "logic.hh"
#include "modes.hh"

Image fb_back(S_WIDTH, S_HEIGHT);
Image fb_front(S_WIDTH, S_HEIGHT);
Color flashColor, fadeColor;
const Color normalizingColor = Color(1, 1, 1);
bool isFading = false;

void FadeReset()
{
    fadeColor = Color::transparent;
}

void FadeResetToBlack()
{
    fadeColor = Color(S_MAXCLR, S_MAXCLR, S_MAXCLR);
}

bool FadeStepOut()
{
    if (fadeColor.getR() >= S_MAXCLR)
        return false;
    fadeColor += normalizingColor;
    return true;
}

bool FadeStepIn()
{
    fadeColor -= normalizingColor;
    return static_cast<bool>(fadeColor);
}

static inline void DrawFrameBack()
{
    switch (activeMode)
    {
    case GameMode::Logo:
        DrawLogoFrame(fb_back); break;
    case GameMode::TitleScreen:
        DrawTitleFrame(fb_back); break;
    case GameMode::Game:
        RenderGame(fb_back); break;
    case GameMode::Ending:
        break;
    case GameMode::NameEntry:
        break;
    }
}

static inline void DrawFrameFront()
{
    std::copy(fb_back.buffer().begin(), fb_back.buffer().end(), 
            fb_front.buffer().begin());
    if (fadeColor)
        fb_front.subtract(fadeColor);
}

void ClearScreen()
{
    std::fill(fb_back.buffer().begin(), fb_back.buffer().end(),
            Color::transparent);
}

void UpdateBackbuffer()
{
    DrawFrameBack();
}

void DrawFrame()
{
    if (!isFading) DrawFrameBack();
    DrawFrameFront();
}
