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
#include "defs.hh"
#include "maths.hh"
#include "render.hh"

class Image
{
    Image(int width, int height, std::istream data);
    void draw(Framebuffer &fb);
    void draw(Framebuffer &fb, int x, int y);
private:
    int _width;
    int _height;
    std::vector<Color> _data;
};

// background layer
class BackgroundLayer
{
    BackgroundLayer(Image &bg, int sx, int sy);
private:
    std::shared_ptr<Image> _bg;
    int _scrollXMul;
    int _scrollYMul;
};

#endif // M_LAYER_HH
