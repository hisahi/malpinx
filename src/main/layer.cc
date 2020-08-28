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
#include "fix.hh"

BackgroundLayer::BackgroundLayer(std::shared_ptr<Image> bg,
                                int ox, int oy, Fix sx, Fix sy)
    : _img(bg), _offsetX(ox), _offsetY(oy), _scrollXMul(sx), _scrollYMul(sy)
{
}

void BackgroundLayer::blit(Image &fb, LayerScroll scroll)
{
    _img->blitTiled(fb, 0, 0, (scroll.x * _scrollXMul).round() - _offsetX,
        (scroll.y * _scrollYMul).round() - _offsetY, S_WIDTH, S_HEIGHT);
}

void NonTiledBackgroundLayer::blit(Image &fb, LayerScroll scroll)
{
    _img->blit(fb, 0, 0, (scroll.x * _scrollXMul).round() - _offsetX,
        (scroll.y * _scrollYMul).round() - _offsetY, S_WIDTH, S_HEIGHT);
}

void HTiledBackgroundLayer::blit(Image &fb, LayerScroll scroll)
{
    int sy = _offsetY - (scroll.y * _scrollYMul).round(), oy = 0, sh = S_HEIGHT;
    if (sy < 0)
    {
        oy -= sy;
        sh += sy;
        oy = 0;
    }
    if (oy + sh > _img->height())
        sh = _img->height() - oy;
    if (sh > 0)
        _img->blitTiled(fb, 0, sy,
            (scroll.x * _scrollXMul).round() - _offsetX, oy, S_WIDTH, sh);
}

void AdditiveBackgroundLayer::blit(Image &fb, LayerScroll scroll)
{
    _img->blitAdditiveTiled(fb, 0, 0,
        (scroll.x * _scrollXMul).round() - _offsetX,
        (scroll.y * _scrollYMul).round() - _offsetY, S_WIDTH, S_HEIGHT);
}

void HTiledAdditiveBackgroundLayer::blit(Image &fb, LayerScroll scroll)
{
    int sy = 0, oy = (scroll.y * _scrollYMul).round() - _offsetY, sh = S_HEIGHT;
    if (oy < 0)
    {
        sy -= oy;
        sh += oy;
        oy = 0;
    }
    if (sy + sh > _img->height())
        sh = _img->height() - sy;
    if (sh > 0)
        _img->blitAdditiveTiled(fb, 0, sy,
            (scroll.x * _scrollXMul).round() - _offsetX, oy, S_WIDTH, sh);
}

void HTiledParallaxBackgroundLayer::blit(Image &fb, LayerScroll scroll)
{
    int sy = _offsetY - (scroll.y * _scrollYMul).round(), oy = 0, sh = S_HEIGHT;
    if (sy < 0)
    {
        oy -= sy;
        sh += sy;
        oy = 0;
    }
    if (oy + sh > _img->height())
        sh = _img->height() - oy;
    Fix truexm = _scrollXMul;
    for (int yoff = 0; yoff < sh; ++yoff)
    {
        _img->blitTiled(fb, 0, sy + yoff,
            (scroll.x * truexm).round() - _offsetX,
            oy + yoff, S_WIDTH, 1);
        truexm += sign * 0.016_x;
    }
}

void HTiledWavyBackgroundLayer::blit(Image &fb, LayerScroll scroll)
{
    int sy = _offsetY - (scroll.y * _scrollYMul).round(), oy = 0, sh = S_HEIGHT;
    if (sy < 0)
    {
        oy -= sy;
        sh += sy;
        oy = 0;
    }
    if (oy + sh > _img->height())
        sh = _img->height() - oy;
    int sinOff = phase;
    for (int yoff = 0; yoff < sh; ++yoff)
    {
        _img->blitTiled(fb, 0, sy + yoff,
            (scroll.x * _scrollXMul
                + 16 * sineTable[sinOff >> 1]).round() - _offsetX,
            oy + yoff, S_WIDTH, 1);
        sinOff = (sinOff + 1) % 256;
    }
    phase = (phase + 1) % 256;
}

ForegroundLayer::ForegroundLayer(std::shared_ptr<Image> bg,
                                int ox, int oy, Fix sx, Fix sy)
    : _img(bg), _offsetX(ox), _offsetY(oy), _scrollXMul(sx), _scrollYMul(sy)
{
}

void ForegroundLayer::blit(Image &fb, LayerScroll scroll)
{
    _img->blitTiled(fb, 0, 0, (scroll.x * _scrollXMul).round() - _offsetX,
        (scroll.y * _scrollYMul).round() - _offsetY, S_WIDTH, S_HEIGHT);
}


bool ForegroundLayer::hitsSprite(Image &spriteImage, LayerScroll scroll,
                const Hitbox &box, Fix spriteX, Fix spriteY) const
{
    return _img->overlapsTiled(spriteImage, 
            (scroll.x + spriteX).round() - _offsetX,
            (scroll.y + spriteY).round() - _offsetY,
            box.x, box.y, box.w, box.h);
}

void NonTiledForegroundLayer::blit(Image &fb, LayerScroll scroll)
{
    _img->blit(fb, 0, 0, (scroll.x * _scrollXMul).round() - _offsetX,
        (scroll.y * _scrollYMul).round() - _offsetY, S_WIDTH, S_HEIGHT);
}

bool NonTiledForegroundLayer::hitsSprite(Image &spriteImage, LayerScroll scroll,
                const Hitbox &box, Fix spriteX, Fix spriteY) const
{
    return _img->overlaps(spriteImage, 
            (scroll.x + spriteX).round() - _offsetX,
            (scroll.y + spriteY).round() - _offsetY,
            box.x, box.y, box.w, box.h);
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

bool ColorWindow::hasColor() const
{
    return static_cast<bool>(_color);
}

bool ColorWindow::fade(int n/* = 1*/)
{
    return static_cast<bool>(_color -= Color(n, n, n));
}

void ColorWindow::flash(Color clr)
{
    _color += clr;
}

FadeWindow::FadeWindow(int x, int y, int w, int h)
    : _x(x), _y(y), _width(w), _height(h)
{
}

void FadeWindow::blit(Image &fb)
{
    Color m = _color;
    if (!m) return;
    int stride = fb.width();
    auto dst = fb.buffer().begin() + (_y * stride + _x);
    int y;
    for (y = 0; y < _height; ++y)
    {
        std::transform(dst, dst + _width, dst,
            [=](const Color &c) { return c - m; });
        dst += stride;
    }
}

bool FadeWindow::hasColor() const
{
    return static_cast<bool>(_color);
}

bool FadeWindow::fadeOut(int n/* = 1*/)
{
    return (_color += Color(n, n, n)).getR() >= S_MAXCLR;
}

bool FadeWindow::fadeIn(int n/* = 1*/)
{
    return !static_cast<bool>(_color -= Color(n, n, n));
}
