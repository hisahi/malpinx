/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// image.cc: class for images and image rendering

#include <vector>
#include "defs.hh"
#include "image.hh"
#include <iostream>

Image::Image(int width, int height)
    : _width(width), _height(height), _data(width * height)
{
}

Image::Image(int width, int height, std::vector<Color> &&data)
    : _width(width), _height(height), _data(data)
{
}

void Image::clear()
{
    std::fill(_data.begin(), _data.end(), Color::transparent);
}

constexpr static std::uint16_t maskTable[] = { 0xffff, 0x0000 };

template <bool tiled, bool fast>
static inline REALLY_INLINE void doBlit(Image &fb,
                int mw, int mh, std::vector<Color> &_data,
                int dx, int dy, int sx, int sy, int sw, int sh)
{
    static_assert(!(tiled && fast), "cannot use tiling with fast blit");

    // sprite clipping
    if (dx < 0)
    {
        sx += dx;
        sw += dx;
        dx = 0;
    }
    if (dy < 0)
    {
        sy += dy;
        sh += dy;
        dy = 0;
    }

    if (tiled)
    {
        sw = std::min({ sw, S_WIDTH - dx });
        sh = std::min({ sh, S_HEIGHT - dy });
    }
    else
    {
        sw = std::min({ sw, mw - sx, S_WIDTH - dx });
        sh = std::min({ sh, mh - sy, S_HEIGHT - dy });
    }

    if (sw <= 0 || sh <= 0) return;

    int fbs = fb.width();
    int xo, yo, stripe_off = fbs - sw;
    auto dst = fb.buffer().begin() + (dy * fbs + dx);
    auto src = _data.begin() + ((sy % mh) * mw + (sx % mw));
    int gap = mw - sx;
    auto row_end = src;
    auto img_end = _data.end();
    int mask;
    for (yo = 0; yo < sh; ++yo)
    {
        row_end = src + sw;
        if (fast)
        {
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
                if (tiled && src == row_end)
                    src -= sw;
            }
            src = row_end;
            dst += stripe_off;
        }
        if (tiled && dst >= img_end)
            dst -= _data.size();
    }
}

void Image::blit(Image &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
    doBlit<false, false>(fb, _width, _height, _data,
            dx, dy, sx, sy, sw, sh);   
}

void Image::blitTiled(Image &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
    doBlit<true, false>(fb, _width, _height, _data,
            dx, dy, sx, sy, sw, sh);
}

void Image::blitFast(Image &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
    doBlit<false, true>(fb, _width, _height, _data,
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
