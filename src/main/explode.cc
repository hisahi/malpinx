/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// explode.cc: explosion object

#include <random>
#include "stage.hh"
#include "explode.hh"

Spritesheet explosionSprites;

ExplosionSprite::ExplosionSprite(int id, Fix x, Fix y, ExplosionSize size,
                                bool center)
    : Sprite(id, nullptr, x, y, SPRITE_NOSCROLL, SpriteType::Explosion)
{
    int i = static_cast<int>(size), si = -1;
    currentFrame = EXPLOSION_FRAMES_START[i];
    lastFrame = currentFrame + EXPLOSION_FRAMES[i];
    _divider = _dividerInitial = EXPLOSION_SPEED[i];
    updateImage(explosionSprites.getImage(currentFrame));
    if (center)
    {
        _x -= _img->width() / 2;
        _y -= _img->height() / 2;
    }
}

void ExplosionSprite::tick()
{
    if (!--_divider)
    {
        if (++currentFrame == lastFrame)
        {
            kill();
            return;
        }
        updateImage(explosionSprites.getImage(currentFrame));
        _divider = _dividerInitial;
    }
}
