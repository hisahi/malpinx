/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// layer.cc: layer implementations

#include <memory>
#include <algorithm>
#include "layer.hh"
#include "sprite.hh"

BackgroundLayer::BackgroundLayer(std::shared_ptr<Image> bg, int sx, int sy)
    : _img(bg), _scrollXMul(sx), _scrollYMul(sy)
{
}

void BackgroundLayer::blit(Image &fb, int sx, int sy) const
{
    _img->blitTiled(fb, 0, 0, sx, sy, S_WIDTH, S_HEIGHT);
}

ColorWindow::ColorWindow(int x, int y, int w, int h)
    : ColorWindow(Color::transparent, x, y, w, h)
{
}

ColorWindow::ColorWindow(Color clr, int x, int y, int w, int h)
    : _color(clr), _x(x), _y(y), _width(w), _height(h)
{
}

void ColorWindow::blit(Image &fb)
{
    Color m = _color;
    if (!m) return;
    int stride = fb.width();
    auto dst = fb.buffer().begin() + (_y * stride + _x);
    int y;
    for (y = 0; y < _height; ++y)
    {
        std::transform(dst, dst + _width, dst,
            [=](const Color &c) { return c + m; });
        dst += stride;
    }
}

bool ColorWindow::fade(int n/* = 1*/)
{
    return static_cast<bool>(_color -= Color(n, n, n));
}

void ColorWindow::flash(Color clr)
{
    _color += clr;
}
