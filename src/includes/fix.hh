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
#include <string>
#include <ostream>

class Fix
{
public:
    constexpr static int SHIFT = 8;
    Fix() : _v(0) { }
    explicit Fix(int i) : _v(i << SHIFT) { }
    explicit Fix(float f) : _v(static_cast<std::int32_t>(f * (1 << SHIFT))) { }
    explicit Fix(double d) : _v(static_cast<std::int32_t>(d * (1 << SHIFT))) { }
    static Fix raw(std::int32_t v) { Fix f; f._v = v; return f; }
    explicit operator int() const { return _v >> SHIFT; }
    explicit operator float() const {
        return static_cast<float>(_v) / (1 << SHIFT); }
    explicit operator double() const {
        return static_cast<double>(_v) / (1 << SHIFT); }
        
    const static Fix PI;
    const static Fix TAU;
    
    int round() const { return static_cast<int>(*this); }
    Fix abs() const { return Fix::raw(_v < 0 ? -_v : _v); }
    std::string str() const {
        return std::to_string(static_cast<float>(*this)); }
    explicit operator std::string() const { return str(); }
    Fix sqrt() const;
    uint32_t raw() const { return _v; }

    friend std::ostream& operator<<(std::ostream &os, const Fix &f)
    {
        return os << f.str();
    }

    friend Fix operator+(const Fix &lhs, const Fix &rhs);
    friend Fix operator-(const Fix &lhs, const Fix &rhs);
    friend Fix operator*(const Fix &lhs, const Fix &rhs);
    friend Fix operator/(const Fix &lhs, const Fix &rhs);

    friend Fix operator+(const Fix &lhs, const int &rhs);
    friend Fix operator-(const Fix &lhs, const int &rhs);
    friend Fix operator*(const Fix &lhs, const int &rhs);
    friend Fix operator/(const Fix &lhs, const int &rhs);

    friend Fix operator+(const int &lhs, const Fix &rhs);
    friend Fix operator-(const int &lhs, const Fix &rhs);
    friend Fix operator*(const int &lhs, const Fix &rhs);
    friend Fix operator/(const int &lhs, const Fix &rhs);

    friend bool operator==(const Fix &lhs, const Fix &rhs);
    friend bool operator!=(const Fix &lhs, const Fix &rhs);
    friend bool operator<(const Fix &lhs, const Fix &rhs);
    friend bool operator<=(const Fix &lhs, const Fix &rhs);
    friend bool operator>(const Fix &lhs, const Fix &rhs);
    friend bool operator>=(const Fix &lhs, const Fix &rhs);

    friend bool operator==(const Fix &lhs, const int &rhs);
    friend bool operator!=(const Fix &lhs, const int &rhs);
    friend bool operator<(const Fix &lhs, const int &rhs);
    friend bool operator<=(const Fix &lhs, const int &rhs);
    friend bool operator>(const Fix &lhs, const int &rhs);
    friend bool operator>=(const Fix &lhs, const int &rhs);

    friend bool operator==(const int &lhs, const Fix &rhs);
    friend bool operator!=(const int &lhs, const Fix &rhs);
    friend bool operator<(const int &lhs, const Fix &rhs);
    friend bool operator<=(const int &lhs, const Fix &rhs);
    friend bool operator>(const int &lhs, const Fix &rhs);
    friend bool operator>=(const int &lhs, const Fix &rhs);

    Fix &operator+=(const Fix &rhs);
    Fix &operator-=(const Fix &rhs);
    Fix &operator*=(const Fix &rhs);
    Fix &operator/=(const Fix &rhs);
    Fix &operator+=(const int &rhs);
    Fix &operator-=(const int &rhs);
    Fix &operator*=(const int &rhs);
    Fix &operator/=(const int &rhs);
    Fix operator-() const { return Fix::raw(-_v); }
    operator bool() const { return static_cast<bool>(_v); }
    bool operator!() const { return !_v; }
private:
    std::int32_t _v;
};

Fix operator "" _x(unsigned long long v);
Fix operator "" _x(long double v);

inline const Fix Fix::PI = 3.141592653589793_x;
inline const Fix Fix::TAU = 2 * Fix::PI;

struct FixPolar2D;
struct Fix2D
{
    Fix2D() : x(0_x), y(0_x) { }
    Fix2D(Fix x, Fix y) : x(x), y(y) { }
    Fix2D(FixPolar2D polar);
    Fix x, y;

    Fix len() const;
    
    friend Fix2D operator+(const Fix2D &lhs, const Fix2D &rhs);
    friend Fix2D operator-(const Fix2D &lhs, const Fix2D &rhs);
    friend Fix2D operator*(const Fix2D &lhs, const Fix &rhs);
    friend Fix2D operator/(const Fix2D &lhs, const Fix &rhs);
    friend Fix2D operator*(const Fix2D &lhs, const int &rhs);
    friend Fix2D operator/(const Fix2D &lhs, const int &rhs);

    Fix2D &operator+=(const Fix2D &rhs);
    Fix2D &operator-=(const Fix2D &rhs);
    Fix2D operator-() const { return Fix2D(-x, -y); }

    friend bool operator==(const Fix2D &lhs, const Fix2D &rhs);
    friend bool operator!=(const Fix2D &lhs, const Fix2D &rhs);
    
    operator bool() const { return x || y; }
    bool operator!() const { return !(x || y); }
};

Fix2D FixNorm(Fix2D xy, Fix length);
Fix2D FixRotate(Fix2D xy, Fix radians);
// "slow" operations
Fix FixSin(Fix radians);
Fix FixCos(Fix radians);
Fix FixAtan2(Fix y, Fix x);

Fix SubtractAngles(Fix a, Fix b);

struct FixPolar2D
{
    FixPolar2D() : length(0_x), angle(0_x) { }
    FixPolar2D(Fix length, Fix angle) : length(length), angle(angle) { }
    FixPolar2D(Fix2D rect);
    Fix length, angle;
};

extern const Fix sineTable[128];
extern const Fix deltaSineTable[128];

// std::min, std::max specialization for Fix
namespace std
{    
    Fix abs(const Fix &n);
    Fix min(const Fix &a, const Fix &b);
    Fix max(const Fix &a, const Fix &b);
};

#endif // M_FIX_HH
