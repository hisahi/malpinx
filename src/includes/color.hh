/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// color.hh: includes for color.cc; the Color class

#ifndef M_COLOR_HH
#define M_COLOR_HH

#include <vector>
#include <cstdint>
#include <array>
#include "defs.hh"
#include "maths.hh"

struct Color
{
    Color() : Color(0) {}
    Color(std::uint16_t v_) : v(v_) { }
    Color(int r, int g, int b) 
        : v((clamp(r, 0, S_MAXCLR) << 8) 
          | (clamp(g, 0, S_MAXCLR) << 4) 
          | (clamp(b, 0, S_MAXCLR)) | 0x8000) { }
    int getR() const { return (v >> 8) & 15; }
    int getG() const { return (v >> 4) & 15; }
    int getB() const { return (v     ) & 15; }
    void setR(int r) { v = (clamp(r, 0, S_MAXCLR) << 8) | (v & 0xF0FF); }
    void setG(int g) { v = (clamp(g, 0, S_MAXCLR) << 4) | (v & 0xFF0F); }
    void setB(int b) { v = (clamp(b, 0, S_MAXCLR)     ) | (v & 0xFFF0); }
    static const Color transparent;
    bool isTransparent() const { return v == 0; }
    explicit operator bool() const { return !isTransparent(); }
    bool operator!() const { return isTransparent(); }
    
    friend Color operator+(const Color& lhs, const Color &rhs);
    friend Color operator-(const Color& lhs, const Color &rhs);

    Color& operator+=(const Color& rhs);
    Color& operator-=(const Color& rhs);

    std::uint16_t v;
};

#endif // M_COLOR_HH
