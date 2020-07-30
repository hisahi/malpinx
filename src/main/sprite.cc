/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sprite.cc: sprite implementations

#include <memory>
#include "sprite.hh"

Sprite::Sprite(std::shared_ptr<Image> img, int x, int y, int flags) :
     _img(img), _x(x), _y(y), _flags(flags)
{
}

void Sprite::blit(Image &fb) const
{
    _img->blit(fb, _x, _y, 0, 0, _img->width(), _img->height());
}

Spritesheet::Spritesheet(const std::vector<std::shared_ptr<Image>> &images)
    : sprites(images)
{
}

std::shared_ptr<Image> Spritesheet::getImage(int index) const
{
    return sprites.at(index);
}

template <class T, class... Args>
T Spritesheet::makeSprite(int index, int x, int y,
        int flags, Args... args) const
{
    static_assert(std::is_base_of<Sprite, T>::value, "must be sprite type");
    return T(sprites.at(index), x, y, flags, std::forward(args...));
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
