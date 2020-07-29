/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// defs.hh: standard definitions

#ifndef M_DEFS_HH
#define M_DEFS_HH

// native screen resolution
constexpr int S_WIDTH = 320;
constexpr int S_HEIGHT = 240;

// ticks per second
constexpr int S_TICKS = 60;
constexpr unsigned long long S_TICK_US = 1000000ULL / S_TICKS;

// number of distinct values in a color channel
constexpr int S_COLORS = 32;
constexpr int S_FADEOUT_MS = (S_COLORS * 1000) / S_TICKS;

constexpr int S_SAMPLE_RATE_LO = 22050;
constexpr int S_SAMPLE_RATE_HI = 44100;

#endif // M_DEFS_HH
