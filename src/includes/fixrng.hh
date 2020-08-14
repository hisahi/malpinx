/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// fixrng.hh: fixed-point RNG (designed to be weakish/consistent)

#ifndef M_FIXRNG_HH
#define M_FIXRNG_HH

#include "fix.hh"
#include "defs.hh"

class FixRandom
{
public:
    FixRandom() : FixRandom(2) { }
    FixRandom(std::uint32_t seed) : seed(seed) { }

private:
    const static std::uint32_t ANGLE_MUL;
    std::uint32_t seed;

    std::uint32_t nextInt()
    {   
        return seed = seed * 259331095 + 732840625;
    }

public:
    void reset(std::uint32_t seed)
    {
        FixRandom::seed = seed;
    }

    Fix nextFix()
    {
        return Fix::raw(nextInt() & ~(~0 << Fix::SHIFT));
    }

    Fix nextAngle()
    {
        std::uint32_t i = (nextInt() >> 3) & ~(~0 << Fix::SHIFT);
        i *= ANGLE_MUL;
        return Fix::raw(i >> Fix::SHIFT);
    }
};

#endif // M_FIXRNG_HH
