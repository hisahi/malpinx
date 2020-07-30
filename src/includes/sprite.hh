/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sprite.hh: includes for sprite.cc

#ifndef M_SPRITE_HH
#define M_SPRITE_HH

#include <vector>
#include "defs.hh"
#include "maths.hh"
#include "render.hh"
#include "layer.hh"

// unpacked 16-bit sprite
class Sprite
{
public:
    Sprite(std::shared_ptr<Image> img);
    void draw(Framebuffer &fb, int x, int y);
private:
    std::shared_ptr<Image> _img;
};

#endif // M_SPRITE_HH
