/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// render.hh: includes for render.cc

#ifndef M_RENDER_HH
#define M_RENDER_HH

#include <array>
#include "defs.hh"
#include "image.hh"

extern Image fb_back;
extern Image fb_front;
extern bool isFading;

void FadeReset();
void FadeResetToBlack();
bool FadeStepOut();
bool FadeStepIn();
void ClearScreen();
void UpdateBackbuffer();
void DrawFrame();

#endif // M_RENDER_HH
