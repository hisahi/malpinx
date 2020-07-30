/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// logic.cc: top-level class for game logic

#include <functional>
#include <iostream>
#include "logic.hh"
#include "modes.hh"
#include "render.hh"
#include "songs.hh"

GameMode activeMode = GameMode::None;
std::function<void()> fadeOutCompleteFunc = nullptr;
bool fadingOut = false, fadingIn = false;
int fadeTicks = 0;

void RunModeBasic()
{
    switch (activeMode)
    {
    case GameMode::Logo:
        RunLogoFrame(); break;
    case GameMode::TitleScreen:
        RunTitleFrame(); break;
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

void StartFadeOut(std::function<void()> onFadeOutDone /*= nullptr*/)
{
    if (fadingOut) return;
    fadeOutCompleteFunc = [=]() {
        if (onFadeOutDone) onFadeOutDone();
        fadeOutCompleteFunc = nullptr;
    };
    fadeTicks = 0;
    if (fadingOut = activeMode != GameMode::None)
    {
        isFading = true;   
        FadeOutSong();
    }
    else
        fadeOutCompleteFunc();
}

void JumpModeInstant(GameMode mode, std::function<void()> init /*= nullptr*/)
{
    activeMode = mode;
    fadeTicks = 0;
    ClearScreen();
    if (init) init();
}

void JumpMode(GameMode mode, std::function<void()> init /*= nullptr*/)
{
    if (fadingOut) return;
    fadeOutCompleteFunc = [=]() {
        activeMode = mode;
        ClearScreen();
        if (init) init();
        UpdateBackbuffer();
        fadingIn = true;
        fadeOutCompleteFunc = nullptr;
        fadeTicks = 0;
    };
    fadeTicks = 0;
    if (fadingOut = activeMode != GameMode::None)
    {
        isFading = true;
        FadeOutSong();
    }
    else
        fadeOutCompleteFunc();
}

void RunFrame()
{
    if (fadingOut)
    {
        if (!fadeTicks && !(fadingOut = FadeStepOut()))
        {
            fadeOutCompleteFunc();
            isFading = fadingOut || fadingIn;
        }
        fadeTicks = (fadeTicks + 1) & 3;
    }
    else if (fadingIn) 
    {
        if (!fadeTicks && !(fadingIn = FadeStepIn()))
        {
            FadeReset();
            isFading = fadingOut || fadingIn;
        }
        fadeTicks = (fadeTicks + 1) & 3;
    }
    if (!isFading)
        RunModeBasic();
}
