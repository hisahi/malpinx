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
#include "maths.hh"
#include "render.hh"

class Image
{
public:
    Image(int width, int height, std::vector<Color> &&data);
    void draw(Framebuffer &fb);
    void draw(Framebuffer &fb, int x, int y);
    void draw(Framebuffer &fb, int dx, int dy, int sx, int sy, int sw, int sh);
    const int &width() const { return _width; }
    const int &height() const { return _height; }
private:
    int _width;
    int _height;
    std::vector<Color> _data;
};

// background layer
class BackgroundLayer
{
public:
    BackgroundLayer(std::shared_ptr<Image> bg, int sx, int sy);
    void draw(Framebuffer &fb, int sx, int sy);
private:
    std::shared_ptr<Image> _img;
    int _scrollXMul;
    int _scrollYMul;
};

#endif // M_LAYER_HH
