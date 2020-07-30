/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// defs.hh: standard definitions

#ifndef M_DEFS_HH
#define M_DEFS_HH

constexpr bool M_DEBUG = true;

// native screen resolution
constexpr int S_WIDTH = 320;
constexpr int S_HEIGHT = 240;

// ticks per second
constexpr int S_TICKS = 60;
constexpr unsigned long long S_TICK_US = 1000000ULL / S_TICKS;

// number of distinct values in a color channel
constexpr int S_COLORS = 16;
constexpr int S_MAXCLR = S_COLORS - 1;
constexpr int S_FADEOUT_MS = (S_COLORS * 1000 * 4) / S_TICKS;

constexpr int S_SAMPLE_RATE_LO = 22050;
constexpr int S_SAMPLE_RATE_HI = 44100;

#if defined(__GNUC__) || defined(__clang__)
#define REALLY_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define REALLY_INLINE __forceinline
#else
#define REALLY_INLINE
#endif

#endif // M_DEFS_HH
