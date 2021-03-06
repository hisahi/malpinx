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
    bool hasColor() const;
    bool fade(int n = 1);
    void flash(Color clr);
private:
    Color _color;
    int _x;
    int _y;
    int _width;
    int _height;
};

// subtractive color layer
class FadeWindow
{
public:
    FadeWindow(int x, int y, int w, int h);
    void blit(Image &fb);
    bool hasColor() const;
    bool fadeIn(int n = 1);
    bool fadeOut(int n = 1);
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
    LayerScroll(Fix _x, Fix _y) : x(_x), y(_y) { }
    Fix x;
    Fix y;
};

// background layer
class BackgroundLayer
{
public:
    BackgroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym);
    virtual void blit(Image &fb, LayerScroll scroll);
    void blitIfShown(Image &fb, LayerScroll scroll)
    {
        if (!_hidden)
            blit(fb, scroll);
    }
    void show()
    {
        _hidden = false;
    }
    void hide()
    {
        _hidden = true;
    }
protected:
    bool _hidden{false};
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
    virtual void blit(Image &fb, LayerScroll scroll);
    virtual bool hitsSprite(Image &spriteImage, LayerScroll scroll,
                const Hitbox &box, Fix spriteX, Fix spriteY) const;
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
    void blit(Image &fb, LayerScroll scroll) override;
};

class HTiledBackgroundLayer : public BackgroundLayer
{
public:
    HTiledBackgroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym)
        : BackgroundLayer(bg, ox, oy, sxm, sym) {}
    void blit(Image &fb, LayerScroll scroll) override;
};

class AdditiveBackgroundLayer : public BackgroundLayer
{
public:
    AdditiveBackgroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym)
        : BackgroundLayer(bg, ox, oy, sxm, sym) {}
    void blit(Image &fb, LayerScroll scroll) override;
};

class HTiledAdditiveBackgroundLayer : public BackgroundLayer
{
public:
    HTiledAdditiveBackgroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym)
        : BackgroundLayer(bg, ox, oy, sxm, sym) {}
    void blit(Image &fb, LayerScroll scroll) override;
};

class HTiledParallaxBackgroundLayer : public BackgroundLayer
{
public:
    HTiledParallaxBackgroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym, int meta)
        : BackgroundLayer(bg, ox, oy, sxm, sym), sign(meta ? -1 : 1) {}
    void blit(Image &fb, LayerScroll scroll) override;
private:
    int sign;
};

class HTiledWavyBackgroundLayer : public BackgroundLayer
{
public:
    HTiledWavyBackgroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym, int meta)
        : BackgroundLayer(bg, ox, oy, sxm, sym) {}
    void blit(Image &fb, LayerScroll scroll) override;
private:
    int phase{0};
};

class NonTiledForegroundLayer : public ForegroundLayer
{
public:
    NonTiledForegroundLayer(std::shared_ptr<Image> bg,
                    int ox, int oy, Fix sxm, Fix sym)
        : ForegroundLayer(bg, ox, oy, sxm, sym) {}
    void blit(Image &fb, LayerScroll scroll) override;
    virtual bool hitsSprite(Image &spriteImage, LayerScroll scroll,
                const Hitbox &box, Fix spriteX, Fix spriteY) const override;
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
    void writeStringTransp
        (const Spritesheet &font, int x, int y, const std::string &s)
    {
        Image &fb = *_img.get();
        int tx = x * FontWidth;
        for (const char &c : s)
        {
            font.blit(fb, c, tx, y * FontHeight);
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
