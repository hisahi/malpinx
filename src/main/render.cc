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

Framebuffer fb_back;
Framebuffer fb_front;
Color flashColor, fadeColor;
const Color normalizingColor = Color(1, 1, 1);

void FadeReset()
{
    fadeColor = Color(0);
}

bool FadeStepOut()
{
    if (fadeColor.getR() < 31)
        return false;
    fadeColor += normalizingColor;
    return true;
}

bool FadeStepIn()
{
    fadeColor -= normalizingColor;
    return fadeColor.v != 0;
}

void DrawFrameBack()
{
    switch (activeMode)
    {
    case GameMode::Logo:
        DrawLogoFrame(fb_back); break;
    case GameMode::TitleScreen:
        break;
    case GameMode::HighScoreScreen:
        break;
    case GameMode::NameEntry:
        break;
    case GameMode::SelectLevel:
        break;
    case GameMode::Cutscene:
        break;
    case GameMode::Game:
        break;
    case GameMode::Credits:
        break;
    }
}

void DrawFrameFront()
{
    if (fadeColor.v)
    {
        std::transform(fb_back.buffer().begin(), fb_back.buffer().end(),
            fb_front.buffer().begin(),
            [](const Color& c) { return c - fadeColor; });
    }
    else if (flashColor.v)
    {
        std::transform(fb_back.buffer().begin(), fb_back.buffer().end(), 
            fb_front.buffer().begin(),
            [](const Color& c) { return c + flashColor; });
        flashColor -= normalizingColor;
    }
    else
    {
        std::copy(fb_back.buffer().begin(), fb_back.buffer().end(), 
                fb_front.buffer().begin());
    }
}

void DrawFrame()
{
    DrawFrameBack();
    DrawFrameFront();
}
