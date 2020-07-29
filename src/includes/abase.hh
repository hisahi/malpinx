/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// abase.hh: header file for base audio library

#ifndef M_ABASE_HH
#define M_ABASE_HH

// guaranteed to be called after vbase_init
void abase_init(int sampleRate);
// guaranteed to be called before vbase_quit
void abase_quit();

#endif // M_ABASE_HH
