/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// ibase.hh: header file for base input library

#ifndef M_IBASE_HH
#define M_IBASE_HH

#include <vector>
#include <string>
#include "defs.hh"
#include "input.hh"

// guaranteed to be called after vbase_init
void ibase_init();
// guaranteed to be called before vbase_quit
void ibase_quit();
void ibase_tick();

bool ibase_menu_down(MenuInput input);
bool ibase_keyb_down(int scanCode);
bool ibase_pad_down(int buttonCode);

int ibase_default_keyb(GameInput input);
int ibase_default_pad(GameInput input);

std::string ibase_get_key_name(int scanCode);
std::string ibase_get_pad_button_name(int buttonCode);

#endif // M_IBASE_HH
