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
#include "input.hh"

GameMode activeMode = GameMode::None;
DifficultyLevel difficulty;
PlaybackMode pmode;
std::function<void()> fadeOutCompleteFunc = nullptr;
bool fadingOut = false, fadingIn = false;
static int fadeTicks = 0;
static int fadeOutId = 0;
static bool fastFade = false;

void RunModeBasic()
{
    switch (activeMode)
    {
    case GameMode::Logo:
        RunLogoFrame(); break;
    case GameMode::TitleScreen:
        RunTitleFrame(); break;
    case GameMode::Game:
        DoGameTick(); break;
    case GameMode::Ending:
        break;
    case GameMode::NameEntry:
        break;
    }
}

void StartFadeOut(std::function<void()> onFadeOutDone /*= nullptr*/)
{
    if (fadingOut) return;
    fadeOutCompleteFunc = [=]() {
        int oldFadeOutId = fadeOutId;
        if (onFadeOutDone) onFadeOutDone();
        if (fadeOutId == oldFadeOutId)
            fadeOutCompleteFunc = nullptr;
    };
    ++fadeOutId;
    fadeTicks = 0;
    fastFade = false;
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
        int oldFadeOutId = fadeOutId;
        activeMode = mode;
        ClearScreen();
        if (init) init();
        UpdateBackbuffer();
        fadingIn = true;
        fastFade = false;
        fadeTicks = 0;
        if (fadeOutId == oldFadeOutId)
            fadeOutCompleteFunc = nullptr;
    };
    ++fadeOutId;
    fadeTicks = 0;
    fastFade = false;
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
        if ((fastFade || !fadeTicks) && !(fadingOut = FadeStepOut()))
        {
            fadeOutCompleteFunc();
            isFading = fadingOut || fadingIn;
            fastFade = false;
        }
        fadeTicks = (fadeTicks + 1) & 3;
        fastFade |= menuInput.select;
    }
    else if (fadingIn) 
    {
        if ((fastFade || !fadeTicks) && !(fadingIn = FadeStepIn()))
        {
            FadeReset();
            isFading = fadingOut || fadingIn;
            fastFade = false;
        }
        fadeTicks = (fadeTicks + 1) & 3;
        fastFade |= menuInput.select;
    }
    if (!isFading)
        RunModeBasic();
}
