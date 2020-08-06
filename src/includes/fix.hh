/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// fix.hh: includes for fixed-point format

#ifndef M_FIX_HH
#define M_FIX_HH

#include <cstdint>
#include <memory>
#include <iostream>
#include "image.hh"
#include "sprite.hh"

class Fix
{
public:
    constexpr static int SHIFT = 8;
    static const std::int32_t ZERO;
    static const std::int32_t ONE;
    static const std::int32_t HALF;
    Fix(std::int32_t v) : _v(v) {};
    Fix(float f) : _v(static_cast<std::int32_t>(f / (1 << SHIFT))) {};
    explicit operator int() const { return _v >> SHIFT; }
    explicit operator float() const {
        return static_cast<float>(_v) / (1 << SHIFT); }
    
    friend Fix operator+(const Fix &lhs, const Fix &rhs);
    friend Fix operator-(const Fix &lhs, const Fix &rhs);
    friend Fix operator*(const Fix &lhs, const Fix &rhs);
    friend Fix operator/(const Fix &lhs, const Fix &rhs);

    Fix& operator+=(const Fix &rhs);
    Fix& operator-=(const Fix &rhs);
    Fix operator-() const { return Fix(-_v); }
    bool operator!() const { return !_v; }
private:
    std::int32_t _v;
};

#endif // M_FIX_HH
