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

void StartFadeOut(std::function<void()> onFadeOutDone /*= nullptr*/)
{
    if (fadingOut) return;
    fadeOutCompleteFunc = [=]() {
        if (onFadeOutDone) onFadeOutDone();
        fadeOutCompleteFunc = nullptr;
    };
    if (fadingOut = activeMode != GameMode::None)
        isFading = true;
    else
        fadeOutCompleteFunc();
}

void JumpModeInstant(GameMode mode, std::function<void()> init /*= nullptr*/)
{
    activeMode = mode;
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
        fadingIn = true;
        fadeOutCompleteFunc = nullptr;
    };
    if (fadingOut = activeMode != GameMode::None)
        isFading = true;
    else
        fadeOutCompleteFunc();
}

void RunFrame()
{
    if (fadingOut)
    {
        if (!(fadingOut = FadeStepOut()))
        {
            fadeOutCompleteFunc();
            isFading = fadingOut || fadingIn;
        }
    }
    else if (fadingIn) 
    {
        if (!(fadingIn = FadeStepIn()))
        {
            FadeReset();
            isFading = fadingOut || fadingIn;
        }
    }
    if (!isFading)
        RunModeBasic();
}
