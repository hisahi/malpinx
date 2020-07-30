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

// additive color layer
class ColorWindow
{
public:
    ColorWindow(Color clr, int x, int y, int w, int h);
    void blit(Image &fb);
    bool fade(int n = 1);
private:
    Color _color;
    int _x;
    int _y;
    int _width;
    int _height;
};

// background layer
class BackgroundLayer
{
public:
    BackgroundLayer(std::shared_ptr<Image> bg, int sx, int sy);
    void blit(Image &fb, int sx, int sy) const;
private:
    std::shared_ptr<Image> _img;
    int _scrollXMul;
    int _scrollYMul;
};

// text layer; consists of non-overlapping sprites
template <int FontWidth, int FontHeight>
class TextLayer
{
public:
    TextLayer() : _img(S_WIDTH, S_HEIGHT) { }
    void blit(Image &fb) const;
    void writeChar(const Spritesheet &font, char c, int x, int y);
private:
    std::unique_ptr<Image> _img;
};

#endif // M_LAYER_HH
