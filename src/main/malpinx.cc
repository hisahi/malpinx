/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// malpinx.cc: main game executable

#include "malpinx.hh"
#include "holders.hh"
#include "logic.hh"
#include "render.hh"
#include "config.hh"
#include "gamedata.hh"

int sampleRate;
GameBackend backend = nullptr;

void DoGame()
{
    LoadConfig();
    backend = GameBackend(GetConfigSampleRate());
    ApplySettingsToBackend();
    OpenDataDir();
    PageInBaseData();
    JumpMode(GameMode::Logo);
    
    while (backend.run())
    {
        RunFrame();
        DrawFrame();
    }

    SaveConfig();
}

void DoArgs(int argc, char **argv)
{

}

int main(int argc, char **argv)
{
    if (argc > 1)
        DoArgs(argc, argv);
    DoGame();
    return 0;
}
