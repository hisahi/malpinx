/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// m_logo.cc: code for logo screens

#include <iostream>
#include <memory>
#include "defs.hh"
#include "layer.hh"
#include "formats.hh"
#include "render.hh"
#include "modes.hh"
#include "logic.hh"
#include "m_logo.hh"
#include "input.hh"

static std::unique_ptr<LogoScreen> logo;

LogoScreen::LogoScreen(int sequence, Image &&image)
    : sequence(sequence), _ticks(S_TICKS * 5),
        _image(std::make_unique<Image>(image))
{
}

bool LogoScreen::fadeOut()
{
    return --_ticks < 0;
}

void LogoScreen::blit(Image &fb)
{
    _image->blit(fb);
}

void InitLogo(int seqnum, const std::string &name)
{
    logo = std::make_unique<LogoScreen>(seqnum, LoadPIC(name));
}

static inline void UnloadLogo()
{
    logo = nullptr;
}

static inline void EndOfLogo()
{
    switch (logo->sequence)
    {
    case 1:
        JumpMode(GameMode::TitleScreen, []() {
            InitTitleScreen(false);
            UnloadLogo();
        });
        break;
    }
}

void DrawLogoFrame(Image &fb)
{
    if (logo)
        logo->blit(fb);
}

void RunLogoFrame()
{
    if (logo->fadeOut() || menuInput.select)
        EndOfLogo();
}
