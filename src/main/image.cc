/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// image.cc: class for images and image rendering

#include <vector>
#include <stdexcept>
#include "defs.hh"
#include "image.hh"
#include "maths.hh"
#include <iostream>

Image::Image(int width, int height)
    : _width(width), _height(height), _data(width * height)
{
}

Image::Image(int width, int height, std::vector<Color> &&data)
    : _width(width), _height(height), _data(data)
{
    if (_data.size() != _width * _height)
        throw std::runtime_error("invalid image data size");
}

void Image::clear()
{
    std::fill(_data.begin(), _data.end(), Color::transparent);
}

constexpr static std::uint16_t maskTable[] = { 0xffff, 0x0000 };

template <bool tiled, bool fast, bool additive>
static inline REALLY_INLINE void doBlit(Image &fb,
                int mw, int mh, std::vector<Color> &_data,
                int dx, int dy, int sx, int sy, int sw, int sh)
{
    static_assert(!(tiled && fast), "cannot use tiling with fast blit");
    static_assert(!additive || fast, "additive must be used with fast blit");

    // sprite clipping
    if (dx < 0)
    {
        sx -= dx;
        sw += dx;
        dx = 0;
    }
    if (dy < 0)
    {
        sy -= dy;
        sh += dy;
        dy = 0;
    }
    if (sw <= 0 || sh <= 0) return;

    int fbs = fb.width(), fbh = fb.height();
    if (tiled)
    {
        sw = std::min({ sw, fbs - dx });
        sh = std::min({ sh, fbh - dy });
    }
    else
    {
        sw = std::min({ sw, mw - sx, fbs - dx });
        sh = std::min({ sh, mh - sy, fbh - dy });
    }
    if (sw <= 0 || sh <= 0) return;

    int xo, yo, stripe_off = fbs - sw;
    auto dst = fb.buffer().begin() + (dy * fbs + dx);
    int srcx = remainder(sx, mw), srcy = remainder(sy, mh);
    auto src = _data.begin() + (srcy * mw + srcx);
    int gap = mw - (sx + sw);
    auto row_end = src, til_nxt = src;
    auto img_end = _data.end();
    int mask;
    for (yo = 0; yo < sh; ++yo)
    {
        row_end = src + sw, til_nxt = src + mw;
        srcx = sx;
        if (fast)
        {
            if (additive)
                std::transform(src, row_end, dst, dst,
                    [](const Color &a, const Color &b) { return a + b; });
            else
                std::copy(src, row_end, dst);
            src += mw;
            dst += fbs;
        }
        else
        {
            for (xo = 0; xo < sw; ++xo, ++src, ++dst)
            {
                mask = maskTable[(*src).isTransparent()];
                *dst = ((*dst).v & ~mask) | ((*src).v & mask);
                //if (!(*src).isTransparent())
                //    *dst = *src;
                if (tiled && ++srcx == mw)
                {
                    src -= mw;
                    srcx -= mw;
                }
            }
            dst += stripe_off;
            src = til_nxt;
            if (tiled && ++srcy == mh)
            {
                src -= _data.size();
                srcy -= mh;
            }
        }
    }
}

void Image::blit(Image &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
    doBlit<false, false, false>(fb, _width, _height, _data,
            dx, dy, sx, sy, sw, sh);   
}

void Image::blitTiled(Image &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
    doBlit<true, false, false>(fb, _width, _height, _data,
            dx, dy, sx, sy, sw, sh);
}

void Image::blitFast(Image &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
    doBlit<false, true, false>(fb, _width, _height, _data,
            dx, dy, sx, sy, sw, sh);
}

void Image::blitAdditive(Image &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
    doBlit<false, true, true>(fb, _width, _height, _data,
            dx, dy, sx, sy, sw, sh);
}

void Image::add(Color color)
{
    std::transform(_data.begin(), _data.end(), _data.begin(),
            [color](const Color &c) { return c + color; });
}

void Image::subtract(Color color)
{
    std::transform(_data.begin(), _data.end(), _data.begin(),
            [color](const Color &c) { return c - color; });
}

void Image::addSolid(Color color)
{
    std::transform(_data.begin(), _data.end(), _data.begin(),
            [color](const Color &c) { return c ? c + color : c; });
}

void Image::subtractSolid(Color color)
{
    std::transform(_data.begin(), _data.end(), _data.begin(),
            [color](const Color &c) { return c ? c - color : c; });
}

// only *this* image will be tiled
template <bool tiled>
static inline REALLY_INLINE bool overlapsImage(Image &other,
                    int mw, int mh, std::vector<Color> &_data,
                    int x, int y, int ox, int oy, int w, int h)
{
    int fbs = other.width();
    auto self = _data.begin() + ((y % mh) * mw + (x % mw));
    auto row_end = self + (mw - (x % mw));
    auto othr = other.buffer().begin() + (y * fbs + x);
    auto img_end = _data.end();
    int self_off = mw - w, othr_off = fbs - w;
    int xo, yo;
    for (yo = 0; yo < h; ++yo)
    {
        for (xo = 0; xo < w; ++xo, ++self, ++othr)
        {
            if (*self && *othr)
                return true;
            if (tiled && self == row_end)
                self -= mw;
        }

        self += self_off;
        othr += othr_off;
        row_end += mw;
        if (tiled && self >= img_end)
        {
            self -= _data.size();
            row_end -= _data.size();
        }
    }

    return false;
}

bool Image::overlaps(Image &other, int x, int y,
                        int ox, int oy, int w, int h)
{
    return overlapsImage<false>(other, _width, _height, _data,
                                x, y, ox, oy, w, h);
}

// only *this* image will be tiled
bool Image::overlapsTiled(Image &other, int x, int y,
                        int ox, int oy, int w, int h)
{
    return overlapsImage<true>(other, _width, _height, _data,
                                x, y, ox, oy, w, h);
}
