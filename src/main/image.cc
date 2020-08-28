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

void Image::fill(Color color)
{
    std::fill(_data.begin(), _data.end(), color);
}

constexpr static std::uint16_t maskTable[] = { 0xffff, 0x0000 };

template <bool tiled, bool fast, bool additive>
static inline REALLY_INLINE void doBlit(Image &fb,
                int mw, int mh, std::vector<Color> &_data,
                int dx, int dy, int sx, int sy, int sw, int sh)
{
    static_assert(!(tiled && fast), "cannot use tiling with fast blit");

    if constexpr (!tiled)
    {
        if (sx < 0)
        {
            dx -= sx;
            sx = 0;
        }
        if (sy < 0)
        {
            dy -= sy;
            sy = 0;
        }
    }

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
    if constexpr (tiled)
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
    int osrcx = remainder(sx, mw), srcx = osrcx, srcy = remainder(sy, mh);
    auto src = _data.begin() + (srcy * mw + srcx);
    int gap = mw - (sx + sw);
    auto row_end = src, til_nxt = src;
    auto img_end = _data.end();
    int mask;
    for (yo = 0; yo < sh; ++yo)
    {
        row_end = src + sw, til_nxt = src + mw;
        srcx = osrcx;
        if constexpr (fast)
        {
            if constexpr (additive)
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
                if constexpr (additive)
                {
                    *dst += *src;
                }
                else
                {
                    mask = maskTable[(*src).isTransparent()];
                    *dst = ((*dst).v & ~mask) | ((*src).v & mask);
                }
                //if (!(*src).isTransparent())
                //    *dst = *src;
                if constexpr (tiled)
                {
                    if (++srcx == mw)
                    {
                        src -= mw;
                        srcx -= mw;
                    }
                }
            }
            dst += stripe_off;
            src = til_nxt;
            if constexpr (tiled)
            {
                if (++srcy == mh)
                {
                    src -= _data.size();
                    srcy -= mh;
                }
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

void Image::blitAdditiveTiled(Image &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
    doBlit<true, false, true>(fb, _width, _height, _data,
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

void Image::addSolid(Color color, int x, int y, int w, int h)
{
    if (x < 0)
    {
        w += x; 
        x = 0;
    }
    if (y < 0)
    {
        h += y;
        y = 0;
    }
    w = std::min(w, _width - x), h = std::min(h, _height - y);
    if (w > 0 && h > 0)
    {
        auto it = _data.begin() + y * _width + x;
        for (int oy = 0; oy < h; ++oy)
        {
            std::transform(it, it + w, it,
                    [color](const Color &c) { return c ? c + color : c; });
            it += _width;
        }
    }
}

void Image::subtractSolid(Color color, int x, int y, int w, int h)
{
    if (x < 0)
    {
        w += x; 
        x = 0;
    }
    if (y < 0)
    {
        h += y;
        y = 0;
    }
    w = std::min(w, _width - x), h = std::min(h, _height - y);
    if (w > 0 && h > 0)
    {
        auto it = _data.begin() + y * _width + x;
        for (int oy = 0; oy < h; ++oy)
        {
            std::transform(it, it + w, it,
                    [color](const Color &c) { return c ? c - color : c; });
            it += _width;
        }
    }
}

// only *this* image will be tiled
template <bool tiled>
static inline REALLY_INLINE bool overlapsImage(Image &other,
                    int mw, int mh, const std::vector<Color> &_data,
                    int x, int y, int ox, int oy, int w, int h)
{
    int fbs = other.width(), fbh = other.height();
    if (!tiled)
    {
        if (x < 0)
        {
            w += x;
            ox -= x;
            x = 0;
        }
        if (y < 0)
        {
            y += h;
            oy -= y;
            y = 0;
        }

        w = std::min({ w, mw - x, fbs - ox });
        h = std::min({ h, mh - y, fbh - oy });
    }

    if (w <= 0 || h <= 0) return false;

    int osrcx = remainder(x, mw), srcx = osrcx, srcy = remainder(y, mh);
    auto self = _data.begin() + (srcy * mw + srcx);
    auto othr = other.buffer().begin() + (oy * fbs + ox);
    int self_off = mw - w, othr_off = fbs - w;
    int xo, yo;
    for (yo = 0; yo < h; ++yo)
    {
        srcx = osrcx;
        for (xo = 0; xo < w; ++xo, ++self, ++othr)
        {
            if (*self && *othr)
                return true;
            if (tiled && ++srcx == mw)
            {
                self -= mw;
                srcx -= mw;
            }
        }

        self += self_off;
        othr += othr_off;
        if (tiled && ++srcy == mh)
        {
            self -= _data.size();
            srcy -= mh;
        }
    }

    return false;
}

bool Image::overlaps(Image &other, int x, int y,
                        int ox, int oy, int w, int h) const
{
    return overlapsImage<false>(other, _width, _height, _data,
                                x, y, ox, oy, w, h);
}

// only *this* image will be tiled
bool Image::overlapsTiled(Image &other, int x, int y,
                        int ox, int oy, int w, int h) const
{
    return overlapsImage<true>(other, _width, _height, _data,
                                x, y, ox, oy, w, h);
}
