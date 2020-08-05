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

ExplosionSprite::ExplosionSprite(int id, int x, int y, ExplosionSize size,
                                bool center)
    : Sprite(id, nullptr, x, y, SPRITE_NOSCROLL, SpriteType::Explosion)
{
    int i, si = -1;
    switch (size)
    {
    case ExplosionSize::Small1:
        i = 0; break;
    case ExplosionSize::Small2:
        i = 1; break;
    case ExplosionSize::Medium1:
        i = 2; break;
    case ExplosionSize::Medium2:
        i = 3; break;
    case ExplosionSize::Large:
        i = 4; break;
    case ExplosionSize::TinyWhite:
        i = 5; break;
    }
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
