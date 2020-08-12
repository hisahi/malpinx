/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// malpinx.cc: main game executable

#include <memory>
#include "malpinx.hh"
#include "backend.hh"
#include "logic.hh"
#include "render.hh"
#include "config.hh"
#include "gamedata.hh"

int sampleRate;
std::unique_ptr<GameBackend> backend;
static bool running;

// always 10 chars           ----------
const std::string VERSION = "PREALPHA 4";

void DoGame()
{
    LoadConfig();
    backend = std::make_unique<GameBackend>(GetConfigSampleRate());
    ApplySettingsToBackend();
    OpenDataDir();
    PageInBaseData();
    FadeResetToBlack();
    JumpMode(GameMode::Logo);
    InitLogo(1, "logo");
    running = true;
    
    while (running && backend->run())
    {
        UpdateInput();
        RunFrame();
        DrawFrame();
        backend->blit();
    }

    SaveConfig();
}

void QuitGame()
{
    running = false;
}

void DoArgs(int argc, char **argv)
{

}

int MalpinxMain(int argc, char **argv)
{
    if (argc > 1)
        DoArgs(argc, argv);
    DoGame();
    return 0;
}

int main(int argc, char **argv)
{
    return MalpinxMain(argc, argv);
}
