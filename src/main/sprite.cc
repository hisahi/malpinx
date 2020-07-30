/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sprite.cc: sprite implementations

#include <memory>
#include "sprite.hh"

Sprite::Sprite(std::shared_ptr<Image> img) : _img(img)
{
}

void Sprite::draw(Framebuffer &fb, int x, int y)
{
    _img->draw(fb, x, y, 0, 0, _img->width(), _img->height());
}
