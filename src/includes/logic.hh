/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// logic.hh: includes for logic.cc

#ifndef M_LOGIC_HH
#define M_LOGIC_HH

#include <functional>
#include "modes.hh"

extern GameMode activeMode;

void StartFadeOut(std::function<void()> onFadeOutDone = nullptr);
void JumpModeInstant(GameMode mode, std::function<void()> init = nullptr);
void JumpMode(GameMode mode, std::function<void()> init = nullptr);
void RunFrame();

#endif // M_LOGIC_HH
