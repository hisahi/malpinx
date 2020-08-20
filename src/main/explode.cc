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
#include "player.hh"

Spritesheet explosionSprites;

ExplosionSprite::ExplosionSprite(Shooter &stg, int id, Fix x, Fix y,
                ExplosionSize size, bool center, bool hurtsPlayer)
    : Sprite(id, nullptr, x, y, SPRITE_COLLIDE_SPRITES, SpriteType::Explosion),
      _stg(stg), hurtsPlayer(hurtsPlayer)
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
    if (hurtsPlayer && _stg.isPlayerAlive() && hits(_stg.player.get()))
        _stg.player->damage(10);
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
