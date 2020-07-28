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

void DoGame()
{
    auto backend = GameBackend();
    while (backend.run())
    {
        RunFrame();
        DrawFrame();
    }
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
