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

ColorWindow::ColorWindow(Color clr, int x, int y, int w, int h)
    : _color(clr), _x(x), _y(y), _width(w), _height(h)
{
}

void ColorWindow::blit(Image &fb)
{
    auto dst = fb.buffer().begin() + (_y * S_STRIDE + _x);
    int y;
    Color m = _color;
    for (y = 0; y < _height; ++y)
    {
        std::transform(dst, dst + _width, dst,
            [=](const Color &c) { return c + m; });
        dst += _height;
    }
}

bool ColorWindow::fade(int n/* = 1*/)
{
    return static_cast<bool>(_color -= Color(n, n, n));
}

template <int FontWidth, int FontHeight>
void TextLayer<FontWidth, FontHeight>::blit(Image &fb) const
{
    _img->blit(fb, 0, 0, 0, 0, S_WIDTH, S_HEIGHT);
}

template <int FontWidth, int FontHeight>
void TextLayer<FontWidth, FontHeight>::writeChar
    (const Spritesheet &font, char c, int x, int y)
{
    font.blitFast(*_img.get(), c, x * FontWidth, y * FontHeight);
}
