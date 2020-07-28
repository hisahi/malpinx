/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// vbase.hh: header file for base graphics library

#ifndef M_VBASE_HH
#define M_VBASE_HH

void vbase_init();
void vbase_set_scale(int scale);
// should game stay running?
bool vbase_loop();
// renders screen
void vbase_flip();
// waits for tick to expire
void vbase_sync();
void vbase_quit();

#endif // M_VBASE_HH
