/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// layer.hh: includes for layer.cc

#ifndef M_LAYER_HH
#define M_LAYER_HH

#include <vector>
#include <istream>
#include <memory>
#include "defs.hh"
#include "image.hh"
#include "maths.hh"
#include "render.hh"
#include "sprite.hh"
#include "fix.hh"

// additive color layer
class ColorWindow
{
public:
    ColorWindow(int x, int y, int w, int h);
    ColorWindow(Color clr, int x, int y, int w, int h);
    void blit(Image &fb);
    bool fade(int n = 1);
    void flash(Color clr);
private:
    Color _color;
    int _x;
    int _y;
    int _width;
    int _height;
};

struct LayerScroll
{
    LayerScroll() : x(0), y(0) { }
    LayerScroll(int _x, int _y) : x(_x), y(_y) { }
    int x;
    int y;
};

// background layer
class BackgroundLayer
{
public:
    BackgroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym);
    virtual void blit(Image &fb, LayerScroll scroll) const;
protected:
    std::shared_ptr<Image> _img;
    int _offsetX;
    int _offsetY;
    Fix _scrollXMul;
    Fix _scrollYMul;
};

// foreground layer
class ForegroundLayer
{
public:
    ForegroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym);
    virtual void blit(Image &fb, LayerScroll scroll) const;
    inline bool hitsSprite(Image &spriteImage, LayerScroll scroll,
                int spriteX, int spriteY) const
    {
        return _img->overlapsTiled(spriteImage,
                            scroll.x + spriteX, scroll.y + spriteY,
                            0, 0,
                            spriteImage.width(), spriteImage.height());
    }
protected:
    std::shared_ptr<Image> _img;
    int _offsetX;
    int _offsetY;
    Fix _scrollXMul;
    Fix _scrollYMul;
};

class NonTiledBackgroundLayer : public BackgroundLayer
{
public:
    NonTiledBackgroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym)
        : BackgroundLayer(bg, ox, oy, sxm, sym) {}
    void blit(Image &fb, LayerScroll scroll) const;
};

class NonTiledForegroundLayer : public ForegroundLayer
{
public:
    NonTiledForegroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym)
        : ForegroundLayer(bg, ox, oy, sxm, sym) {}
    void blit(Image &fb, LayerScroll scroll) const;
};

// text layer; consists of non-overlapping sprites
template <int FontWidth, int FontHeight>
class TextLayer
{
public:
    constexpr static int Columns = (S_WIDTH + FontWidth - 1) / FontWidth;
    constexpr static int Rows = (S_HEIGHT + FontHeight - 1) / FontHeight;
    TextLayer() : _img(std::make_unique<Image>(S_WIDTH, S_HEIGHT)),
            _textBuf() { 
        clear();
    }
    void blit(Image &fb) const
    {
        _img->blit(fb, 0, 0, 0, 0, S_WIDTH, S_HEIGHT);
    }
    void clear()
    {
        _img->clear();
    }
    void writeChar(const Spritesheet &font, int x, int y, char c)
    {
        font.blitFast(*_img.get(), c, x * FontWidth, y * FontHeight);
    }
    void writeString
        (const Spritesheet &font, int x, int y, const std::string &s)
    {
        Image &fb = *_img.get();
        int tx = x * FontWidth;
        for (const char &c : s)
        {
            font.blitFast(fb, c, tx, y * FontHeight);
            if (tx > S_WIDTH)
                break;
            tx += FontWidth;
        }
    }
    void writeStringRightAlign
        (const Spritesheet &font, int x, int y, const std::string &s)
    {
        writeString(font, x - s.length() + 1, y, s);
    }

private:
    std::unique_ptr<Image> _img;
    std::array<char, Rows * Columns> _textBuf;
};

#endif // M_LAYER_HH
