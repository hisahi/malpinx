/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// logic.cc: top-level class for game logic

#include <functional>
#include "logic.hh"
#include "modes.hh"
#include "render.hh"

GameMode activeMode = GameMode::None;
std::function<void()> fadeOutCompleteFunc = nullptr;
bool fadingOut = false, fadingIn = false;

void RunModeBasic()
{
    switch (activeMode)
    {
    case GameMode::Logo:
        RunLogoFrame(); break;
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

void JumpModeInstant(GameMode mode, std::function<void()> init /*= nullptr*/)
{
    activeMode = mode;
    if (init) init();
}

void JumpMode(GameMode mode, std::function<void()> init /*= nullptr*/)
{
    fadeOutCompleteFunc = [=]() {
        if (init) init();
        fadingIn = true;
        fadeOutCompleteFunc = nullptr;
    };
    if (fadingOut = mode != GameMode::None)
        is_fading = true;
    else
        fadeOutCompleteFunc();
}

void RunFrame()
{
    if (fadingOut)
    {
        if (!(fadingOut = FadeStepOut()))
            fadeOutCompleteFunc();
        is_fading = fadingOut || fadingIn;
    }
    else if (fadingIn) 
    {
        if (!(fadingIn = FadeStepIn()))
            FadeReset();
        is_fading = fadingOut || fadingIn;
    }
    else RunModeBasic();
}
