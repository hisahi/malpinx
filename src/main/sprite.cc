/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sprite.cc: sprite implementations

#include <memory>
#include "sprite.hh"
#include "layer.hh"

extern const int gridPoints[256];

Sprite::Sprite(int id, std::shared_ptr<Image> img, int x, int y, int flags)
    : _id(id), _img(img), _x(x), _y(y), _flags(flags),
      _width(img->width()), _height(img->height()), _ticks(0), _dead(false)
{
}

void Sprite::blit(Image &fb) const
{
    _img->blit(fb, _x, _y, 0, 0, _img->width(), _img->height());
}

void Sprite::updateImage(const std::shared_ptr<Image>& img)
{
    _img = img;
    _width = img->width();
    _height = img->height();
}

void Sprite::computeCollisionGrid()
{
    if (_dead) 
    {
        _colgrid = 0;
        return;
    }
    constexpr int xDiv = (S_WIDTH / 4), yDiv = (S_HEIGHT / 4);
    int xBase = std::max(_x, 0) / xDiv,
        yBase = std::min(_y, 0) / yDiv;
    int xEnd = std::min(_x + _width, S_WIDTH - 1) / xDiv,
        yEnd = std::min(_y + _height, S_HEIGHT - 1) / yDiv;
    // 0 <= xBase, yBase, xEnd, yEnd <= 3
    // xBase <= xEnd
    // yBase <= yEnd
    _colgrid = gridPoints[(yBase << 6) | (yEnd << 4) | (xBase << 2) | xEnd];
}

bool Sprite::hitsForeground(ForegroundLayer &layer, LayerScroll scroll)
{
    return layer.hitsSprite(*_img, scroll, _x, _y);
}

bool Sprite::boxCheck(Sprite &other)
{
    int myLeft = _x, myTop = _y;
    int myRight = _x + _width, myBottom = _y + _height;
    int otherLeft = other._x, otherTop = other._y;
    int otherRight = other._x + other._width;
    int otherBottom = other._y + other._height;
    return myLeft < otherRight  && otherLeft < myRight
        && myTop  < otherBottom && otherTop  < myBottom;
}

bool Sprite::pixelCheck(Sprite &other)
{
    int minX = std::max(_x, other._x);
    int maxX = std::min(_x + _width, other._x + other._width);
    int minY = std::max(_y, other._y);
    int maxY = std::min(_y + _height, other._y + other._height);
    return _img->overlaps(*other._img,
                            minX - _x, minY - _y,
                            minX - other._x, minY - other._y,
                            maxX - minX, maxY - minY);
}

Spritesheet::Spritesheet() : sprites()
{
}

Spritesheet::Spritesheet(const std::vector<std::shared_ptr<Image>> &images)
    : sprites(images)
{
}

std::shared_ptr<Image> Spritesheet::getImage(int index) const
{
    return sprites.at(index);
}

void Spritesheet::pageIn(std::shared_ptr<Image> img)
{
    sprites.push_back(img);
}

void Spritesheet::blit(Image &fb, int index, int x, int y) const
{
    sprites.at(index)->blit(fb, x, y);
}

void Spritesheet::blitFast(Image &fb, int index, int x, int y) const
{
    Image &img = *sprites.at(index).get();
    img.blitFast(fb, x, y, 0, 0, img.width(), img.height());
}

// a 8-bit => 16-bit table.
//  input values: yyYYxxXX
//      xx = lowest X coordinate 0-3, XX = highest X coordinate 0-3,
//      yy = lowest Y coordinate 0-3, YY = highest Y coordinate 0-3
//  output values: 0123456789ABCDEF
//      0-F: 0 = X/Y region 0 (X=0,Y=0), 1 = X=1,Y=0, 4 = X=0,Y=1, etc.
const int gridPoints[256] = {
    0x8000, 0xC000, 0xE000, 0xF000, 0x0000, 0x4000, 0x6000, 0x7000,
    0x0000, 0x0000, 0x2000, 0x3000, 0x0000, 0x0000, 0x0000, 0x1000,
    0x8800, 0xCC00, 0xEE00, 0xFF00, 0x0000, 0x4400, 0x6600, 0x7700,
    0x0000, 0x0000, 0x2200, 0x3300, 0x0000, 0x0000, 0x0000, 0x1100,
    0x8880, 0xCCC0, 0xEEE0, 0xFFF0, 0x0000, 0x4440, 0x6660, 0x7770,
    0x0000, 0x0000, 0x2220, 0x3330, 0x0000, 0x0000, 0x0000, 0x1110,
    0x8888, 0xCCCC, 0xEEEE, 0xFFFF, 0x0000, 0x4444, 0x6666, 0x7777,
    0x0000, 0x0000, 0x2222, 0x3333, 0x0000, 0x0000, 0x0000, 0x1111,
    
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0800, 0x0C00, 0x0E00, 0x0F00, 0x0000, 0x0400, 0x0600, 0x0700,
    0x0000, 0x0000, 0x0200, 0x0300, 0x0000, 0x0000, 0x0000, 0x0100,
    0x0880, 0x0CC0, 0x0EE0, 0x0FF0, 0x0000, 0x0440, 0x0660, 0x0770,
    0x0000, 0x0000, 0x0220, 0x0330, 0x0000, 0x0000, 0x0000, 0x0110,
    0x0888, 0x0CCC, 0x0EEE, 0x0FFF, 0x0000, 0x0444, 0x0666, 0x0777,
    0x0000, 0x0000, 0x0222, 0x0333, 0x0000, 0x0000, 0x0000, 0x0111,
    
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0080, 0x00C0, 0x00E0, 0x00F0, 0x0000, 0x0040, 0x0060, 0x0070,
    0x0000, 0x0000, 0x0020, 0x0030, 0x0000, 0x0000, 0x0000, 0x0010,
    0x0088, 0x00CC, 0x00EE, 0x00FF, 0x0000, 0x0044, 0x0066, 0x0077,
    0x0000, 0x0000, 0x0022, 0x0033, 0x0000, 0x0000, 0x0000, 0x0011,
    
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0008, 0x000C, 0x000E, 0x000F, 0x0000, 0x0004, 0x0006, 0x0007,
    0x0000, 0x0000, 0x0002, 0x0003, 0x0000, 0x0000, 0x0000, 0x0001,
};
