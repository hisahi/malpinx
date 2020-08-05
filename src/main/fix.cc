/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// fix.cc: implementation of fixed-point operators

#include <cstdint>
#include "fix.hh"

const std::int32_t Fix::ZERO = 0;
const std::int32_t Fix::ONE = 1 << Fix::SHIFT;
const std::int32_t Fix::HALF = Fix::ONE >> 1;

Fix operator+(const Fix &lhs, const Fix &rhs)
{
    return Fix(lhs._v + rhs._v);
}

Fix operator-(const Fix &lhs, const Fix &rhs)
{
    return Fix(lhs._v - rhs._v);
}

Fix operator*(const Fix &lhs, const Fix &rhs)
{
    return Fix((lhs._v * rhs._v) >> Fix::SHIFT);
}

Fix operator/(const Fix &lhs, const Fix &rhs)
{
    return Fix(static_cast<std::int32_t>(
            static_cast<std::int64_t>(lhs._v << Fix::SHIFT) / rhs._v));
}

Fix& Fix::operator+=(const Fix &rhs)
{
    _v += rhs._v;
    return *this;
}

Fix& Fix::operator-=(const Fix &rhs)
{
    _v -= rhs._v;
    return *this;
}
