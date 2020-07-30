/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// layer.cc: image and layer implementations

#include <memory>
#include <algorithm>
#include "layer.hh"

Image::Image(int width, int height, std::vector<Color> &&data)
    : _width(width), _height(height), _data()
{
    _data.reserve(data.size());
    std::copy(data.begin(), data.end(), _data.begin());
}

void Image::draw(Framebuffer &fb)
{
    draw(fb, 0, 0);
}

void Image::draw(Framebuffer &fb, int x, int y)
{
    draw(fb, 0, 0, 0, 0, _width, _height);
}

void Image::draw(Framebuffer &fb, int dx, int dy,
                int sx, int sy, int sw, int sh)
{
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
    sw = std::min({ sw, _width - sx, S_WIDTH - sx });
    sh = std::min({ sh, _height - sy, S_HEIGHT - sy });

    if (sw <= 0 || sh <= 0) return;

    int xo, yo, stripe_off = S_WIDTH - sw;
    auto dst = fb.buffer().begin() + (dy * S_STRIDE + dx);
    auto src = _data.begin() + (sy * _width + sx);
    for (yo = 0; yo < sh; ++yo)
    {
        for (xo = 0; xo < sw; ++xo, ++src, ++dst)
            if (!(*src).isTransparent())
                *dst = *src;

        dst += stripe_off;
        src += stripe_off;
    }
}

BackgroundLayer::BackgroundLayer(std::shared_ptr<Image> bg, int sx, int sy)
    : _img(bg), _scrollXMul(sx), _scrollYMul(sy)
{
}

void BackgroundLayer::draw(Framebuffer &fb, int sx, int sy)
{
    _img->draw(fb, 0, 0, sx, sy, S_WIDTH, S_HEIGHT);
}
