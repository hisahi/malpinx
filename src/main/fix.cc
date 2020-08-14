/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// fix.cc: implementation of fixed-point operators

#include <cstdint>
#include "fix.hh"
#include "fixrng.hh"
#include "defs.hh"
#include <cmath>

const Fix Fix::PI = 3.141592653589793_x;
const std::uint32_t FixRandom::ANGLE_MUL = (2 * Fix::PI).raw();

Fix operator+(const Fix &lhs, const Fix &rhs)
{
    return Fix::raw(lhs._v + rhs._v);
}

Fix operator-(const Fix &lhs, const Fix &rhs)
{
    return Fix::raw(lhs._v - rhs._v);
}

Fix operator*(const Fix &lhs, const Fix &rhs)
{
    return Fix::raw(static_cast<std::int32_t>((
            static_cast<std::int64_t>(lhs._v) * rhs._v) >> Fix::SHIFT));
}

Fix operator/(const Fix &lhs, const Fix &rhs)
{
    return Fix::raw(static_cast<std::int32_t>(
            static_cast<std::int64_t>(lhs._v << Fix::SHIFT) / rhs._v));
}

Fix operator+(const Fix &lhs, const int &rhs)
{
    return lhs + Fix(rhs);
}

Fix operator-(const Fix &lhs, const int &rhs)
{
    return lhs - Fix(rhs);
}

Fix operator*(const Fix &lhs, const int &rhs)
{
    return lhs * Fix(rhs);
}

Fix operator/(const Fix &lhs, const int &rhs)
{
    return lhs / Fix(rhs);
}

Fix operator+(const int &lhs, const Fix &rhs)
{
    return Fix(lhs) + rhs;
}

Fix operator-(const int &lhs, const Fix &rhs)
{
    return Fix(lhs) - rhs;
}

Fix operator*(const int &lhs, const Fix &rhs)
{
    return Fix(lhs) * rhs;
}

Fix operator/(const int &lhs, const Fix &rhs)
{
    return Fix(lhs) / rhs;
}

Fix &Fix::operator+=(const Fix &rhs)
{
    _v += rhs._v;
    return *this;
}

Fix &Fix::operator-=(const Fix &rhs)
{
    _v -= rhs._v;
    return *this;
}

Fix &Fix::operator*=(const Fix &rhs)
{
    _v = static_cast<std::int32_t>((
            static_cast<std::int64_t>(_v) * rhs._v) >> Fix::SHIFT);
    return *this;
}

Fix &Fix::operator/=(const Fix &rhs)
{
    _v = static_cast<std::int32_t>(
            static_cast<std::int64_t>(_v << Fix::SHIFT) / rhs._v);
    return *this;
}

Fix &Fix::operator+=(const int &rhs)
{
    *this += Fix(rhs);
    return *this;
}

Fix &Fix::operator-=(const int &rhs)
{
    *this -= Fix(rhs);
    return *this;
}

Fix &Fix::operator*=(const int &rhs)
{
    *this *= Fix(rhs);
    return *this;
}

Fix &Fix::operator/=(const int &rhs)
{
    *this /= Fix(rhs);
    return *this;
}

bool operator==(const Fix &lhs, const Fix &rhs)
{
    return lhs._v == rhs._v;
}

bool operator!=(const Fix &lhs, const Fix &rhs)
{
    return lhs._v != rhs._v;
}

bool operator<(const Fix &lhs, const Fix &rhs)
{
    return lhs._v < rhs._v;
}

bool operator<=(const Fix &lhs, const Fix &rhs)
{
    return lhs._v <= rhs._v;
}

bool operator>(const Fix &lhs, const Fix &rhs)
{
    return lhs._v > rhs._v;
}

bool operator>=(const Fix &lhs, const Fix &rhs)
{
    return lhs._v >= rhs._v;
}

bool operator==(const Fix &lhs, const int &rhs)
{
    return lhs._v == Fix(rhs)._v;
}

bool operator!=(const Fix &lhs, const int &rhs)
{
    return lhs._v != Fix(rhs)._v;
}

bool operator<(const Fix &lhs, const int &rhs)
{
    return lhs._v < Fix(rhs)._v;
}

bool operator<=(const Fix &lhs, const int &rhs)
{
    return lhs._v <= Fix(rhs)._v;
}

bool operator>(const Fix &lhs, const int &rhs)
{
    return lhs._v > Fix(rhs)._v;
}

bool operator>=(const Fix &lhs, const int &rhs)
{
    return lhs._v >= Fix(rhs)._v;
}

bool operator==(const int &lhs, const Fix &rhs)
{
    return Fix(lhs)._v == rhs._v;
}

bool operator!=(const int &lhs, const Fix &rhs)
{
    return Fix(lhs)._v != rhs._v;
}

bool operator<(const int &lhs, const Fix &rhs)
{
    return Fix(lhs)._v < rhs._v;
}

bool operator<=(const int &lhs, const Fix &rhs)
{
    return Fix(lhs)._v <= rhs._v;
}

bool operator>(const int &lhs, const Fix &rhs)
{
    return Fix(lhs)._v > rhs._v;
}

bool operator>=(const int &lhs, const Fix &rhs)
{
    return Fix(lhs)._v >= rhs._v;
}

Fix2D operator+(const Fix2D &lhs, const Fix2D &rhs)
{
    return Fix2D(lhs.x + rhs.x, lhs.y + rhs.y);
}

Fix2D operator-(const Fix2D &lhs, const Fix2D &rhs)
{
    return Fix2D(lhs.x - rhs.x, lhs.y - rhs.y);
}

Fix2D operator*(const Fix2D &lhs, const int &rhs)
{
    return Fix2D(lhs.x * rhs, lhs.y * rhs);
}

Fix2D operator/(const Fix2D &lhs, const int &rhs)
{
    return Fix2D(lhs.x / rhs, lhs.y / rhs);
}

Fix2D operator*(const Fix2D &lhs, const Fix &rhs)
{
    return Fix2D(lhs.x * rhs, lhs.y * rhs);
}

Fix2D operator/(const Fix2D &lhs, const Fix &rhs)
{
    return Fix2D(lhs.x / rhs, lhs.y / rhs);
}

Fix2D &Fix2D::operator+=(const Fix2D &rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Fix2D &Fix2D::operator-=(const Fix2D &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Fix Fix2D::len() const
{
    return (x * x + y * y).sqrt();
}

bool operator==(const Fix2D &lhs, const Fix2D &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Fix2D &lhs, const Fix2D &rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

Fix operator "" _x(unsigned long long v)
{
    return Fix(static_cast<int>(v));
};

Fix operator "" _x(long double v)
{
    return Fix(static_cast<float>(v));
};

// std::min, std::max specialization for Fix
namespace std
{    
    Fix min(const Fix &a, const Fix &b)
    {
        return a < b ? a : b;
    }

    Fix max(const Fix &a, const Fix &b)
    {
        return a > b ? a : b;
    }
};

Fix2D::Fix2D(FixPolar2D polar) : x(polar.length * FixCos(polar.angle)),
                                 y(polar.length * FixSin(polar.angle)) { }

FixPolar2D::FixPolar2D(Fix2D rect) : length(rect.len()),
                                     angle(FixAtan2(rect.y, rect.x)) { }

Fix2D FixNorm(Fix2D xy, Fix length)
{
    Fix adj = (length / xy.len());
    if (adj == 0) return Fix2D();
    return xy * adj;
};

Fix2D FixRotate(Fix2D xy, Fix radians)
{
    FixPolar2D polar(xy);
    polar.angle += radians;
    return Fix2D(polar);
};

// "slow" operation
Fix Fix::sqrt() const
{
    if (USE_DOUBLE_MATH)
        return Fix(std::sqrt(static_cast<double>(*this)));
    else
        return Fix(std::sqrt(static_cast<float>(*this)));
}

// "slow" operation
Fix FixSin(Fix radians)
{
    if (USE_DOUBLE_MATH)
        return Fix(std::sin(static_cast<double>(radians)));
    else
        return Fix(std::sin(static_cast<float>(radians)));
};

// "slow" operation
Fix FixCos(Fix radians)
{
    if (USE_DOUBLE_MATH)
        return Fix(std::cos(static_cast<double>(radians)));
    else
        return Fix(std::cos(static_cast<float>(radians)));
};

// "slow" operation
Fix FixAtan2(Fix y, Fix x)
{
    if (x == 0 && y == 0) return 0_x;
    if (USE_DOUBLE_MATH)
        return Fix(std::atan2(static_cast<double>(y), static_cast<double>(x)));
    else
        return Fix(std::atan2(static_cast<float>(y), static_cast<float>(x)));
};

const Fix cosineTable[128] = {
    1.00000_x, 0.99880_x, 0.99518_x, 0.98918_x,
    0.98079_x, 0.97003_x, 0.95694_x, 0.94154_x,
    0.92388_x, 0.90399_x, 0.88192_x, 0.85773_x,
    0.83147_x, 0.80321_x, 0.77301_x, 0.74095_x,
    0.70711_x, 0.67156_x, 0.63439_x, 0.59570_x,
    0.55557_x, 0.51410_x, 0.47140_x, 0.42756_x,
    0.38268_x, 0.33689_x, 0.29028_x, 0.24298_x,
    0.19509_x, 0.14673_x, 0.09802_x, 0.04907_x,
    -0.00000_x, -0.00120_x, -0.00482_x, -0.01082_x,
    -0.01921_x, -0.02997_x, -0.04306_x, -0.05846_x,
    -0.07612_x, -0.09601_x, -0.11808_x, -0.14227_x,
    -0.16853_x, -0.19679_x, -0.22699_x, -0.25905_x,
    -0.29289_x, -0.32844_x, -0.36561_x, -0.40430_x,
    -0.44443_x, -0.48590_x, -0.52860_x, -0.57244_x,
    -0.61732_x, -0.66311_x, -0.70972_x, -0.75702_x,
    -0.80491_x, -0.85327_x, -0.90198_x, -0.95093_x,
    -1.00000_x, -0.99880_x, -0.99518_x, -0.98918_x,
    -0.98079_x, -0.97003_x, -0.95694_x, -0.94154_x,
    -0.92388_x, -0.90399_x, -0.88192_x, -0.85773_x,
    -0.83147_x, -0.80321_x, -0.77301_x, -0.74095_x,
    -0.70711_x, -0.67156_x, -0.63439_x, -0.59570_x,
    -0.55557_x, -0.51410_x, -0.47140_x, -0.42756_x,
    -0.38268_x, -0.33689_x, -0.29028_x, -0.24298_x,
    -0.19509_x, -0.14673_x, -0.09802_x, -0.04907_x,
    0.00000_x, 0.00120_x, 0.00482_x, 0.01082_x,
    0.01921_x, 0.02997_x, 0.04306_x, 0.05846_x,
    0.07612_x, 0.09601_x, 0.11808_x, 0.14227_x,
    0.16853_x, 0.19679_x, 0.22699_x, 0.25905_x,
    0.29289_x, 0.32844_x, 0.36561_x, 0.40430_x,
    0.44443_x, 0.48590_x, 0.52860_x, 0.57244_x,
    0.61732_x, 0.66311_x, 0.70972_x, 0.75702_x,
    0.80491_x, 0.85327_x, 0.90198_x, 0.95093_x
};
