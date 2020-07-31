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
                if (!(*src).isTransparent())
                    *dst = *src;
                if (tiled && src == row_end)
                    src -= sw;
            }
            src = row_end;
            dst += stripe_off;
        }
        if (tiled && dst >= _data.end())
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
