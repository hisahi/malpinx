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

static int logoFrames = 0;
static int logoSequence = 0;
static std::unique_ptr<Image> logoScreen;

void InitLogo(int seqnum, const std::string &name)
{
    logoFrames = S_TICKS * 5;
    logoSequence = seqnum;
    logoScreen = std::move(LoadPIC(name));
}

static inline void UnloadLogo()
{
    logoScreen = nullptr;
}

static inline void EndOfLogo()
{
    switch (logoSequence)
    {
    case 0:
        JumpMode(GameMode::TitleScreen, []() {
            InitTitleScreen();
            UnloadLogo();
        });
        break;
    }
}

void DrawLogoFrame(Framebuffer &fb)
{
    logoScreen->draw(fb);
}

void RunLogoFrame()
{
    if (--logoFrames <= 0)
        EndOfLogo();
}
