/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// render.hh: includes for render.cc

#ifndef M_RENDER_HH
#define M_RENDER_HH

#include <cstdint>
#include <array>
#include "defs.hh"
#include "maths.hh"

struct Color
{
    Color() : Color(0) {}
    Color(std::uint16_t v_) : v(v_) { }
    Color(int r, int g, int b) 
        : v((clamp(r, 0, 31) << 10) 
          | (clamp(g, 0, 31) << 5) 
          | (clamp(b, 0, 31))) { }
    int getR() const { return (v >> 10) & 31; }
    int getG() const { return (v >>  5) & 31; }
    int getB() const { return (v      ) & 31; }
    void setR(int r) { v = (clamp(r, 0, 31) << 10) | (v & 0x03FF); }
    void setG(int g) { v = (clamp(g, 0, 31) <<  5) | (v & 0x7C1F); }
    void setB(int b) { v = (clamp(b, 0, 31)      ) | (v & 0x7FE0); }
    bool isTransparent() const { return v == 0xFFFF; }
    static Color transparent() { return Color(0xFFFF); }
    
    friend Color operator+(const Color& lhs, const Color &rhs);
    friend Color operator-(const Color& lhs, const Color &rhs);

    Color& operator+=(const Color& rhs)
    {
        setR(getR() + rhs.getR());
        setG(getG() + rhs.getG());
        setB(getB() + rhs.getB());
        return *this;
    }

    Color& operator-=(const Color& rhs)
    {
        setR(getR() - rhs.getR());
        setG(getG() - rhs.getG());
        setB(getB() - rhs.getB());
        return *this;
    }

    std::uint16_t v;
};

constexpr int S_STRIDE = S_WIDTH;

class Framebuffer
{
public:
    std::array<Color, S_WIDTH * S_HEIGHT> &buffer() { return _buf; }
private:
    std::array<Color, S_WIDTH * S_HEIGHT> _buf;
};

extern Framebuffer fb_back;
extern Framebuffer fb_front;

void FadeReset();
bool FadeStepOut();
bool FadeStepIn();
void DrawFrame();

#endif // M_RENDER_HH
