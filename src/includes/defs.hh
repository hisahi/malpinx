/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// defs.hh: standard definitions

#ifndef M_DEFS_HH
#define M_DEFS_HH

#include <string>
#include <iostream>
#include <type_traits>

extern const std::string VERSION;
constexpr bool M_DEBUG = true;

// native screen resolution
constexpr int S_WIDTH = 320;
constexpr int S_HEIGHT = 240;

constexpr int S_HUDHEIGHT = 32;
constexpr int S_GHEIGHT = S_HEIGHT - S_HUDHEIGHT;

// ticks per second
constexpr int S_TICKS = 60;
constexpr unsigned long long S_TICK_US = 1000000ULL / S_TICKS;

// number of distinct values in a color channel
constexpr int S_COLORS = 16;
constexpr int S_MAXCLR = S_COLORS - 1;
constexpr int S_FADEOUT_MS = (S_COLORS * 1000 * 4) / S_TICKS;

constexpr int S_SAMPLE_RATE_LO = 22050;
constexpr int S_SAMPLE_RATE_HI = 44100;

template <bool debug = M_DEBUG, typename... Args,
            typename std::enable_if<debug>::type* = nullptr>
inline void DEBUG_LOG(Args... ignore) { }

template <bool debug = M_DEBUG, typename T,
            typename std::enable_if<debug>::type* = nullptr>
inline void DEBUG_LOG(T value)
{
    std::cerr << value << std::endl;
}

template <bool debug = M_DEBUG, typename T, typename... Args,
            typename std::enable_if<debug>::type* = nullptr>
inline void DEBUG_LOG(T head, Args... tail)
{
    std::cerr << head;
    DEBUG_LOG(tail...);
}

#if defined(__GNUC__) || defined(__clang__)
#define REALLY_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define REALLY_INLINE __forceinline
#else
#define REALLY_INLINE
#endif

#endif // M_DEFS_HH
