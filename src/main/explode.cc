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
#include "sfx.hh"

Spritesheet explosionSprites;
static std::random_device rngdev;
static std::mt19937 rngengine(rngdev());
static std::array<SoundEffect, 6> explosionSounds = {
    SoundEffect::ExplosionSmall1, SoundEffect::ExplosionSmall2,
    SoundEffect::ExplosionMedium1, SoundEffect::ExplosionMedium2,
    SoundEffect::ExplosionLarge1, SoundEffect::ExplosionLarge2
};

static inline int randomNumber(int a, int b)
{
    return (std::uniform_int_distribution<std::mt19937::result_type>(a, b))
                                                                (rngengine);
}

ExplosionSprite::ExplosionSprite(int id, int x, int y, ExplosionSize size)
    : Sprite(id, nullptr, x, y, SPRITE_DEFAULT), _divider(true)
{
    int i, si = -1;
    switch (size)
    {
    case ExplosionSize::SMALL:
        si = randomNumber(0, 1); i = randomNumber(0, 1); break;
    case ExplosionSize::MEDIUM:
        si = randomNumber(2, 3); i = randomNumber(2, 3); break;
    case ExplosionSize::LARGE:
        si = randomNumber(4, 5); i = 4; break;
    case ExplosionSize::TINY_WHITE:
        i = 5; break;
    }
    if (si >= 0)
        PlaySound(explosionSounds[si]);
    currentFrame = EXPLOSION_FRAMES_START[i];
    lastFrame = currentFrame + EXPLOSION_FRAMES[i];
    _img = explosionSprites.getImage(currentFrame);
}

void ExplosionSprite::tick()
{
    if (_divider = !_divider)
    {
        if (++currentFrame == lastFrame)
        {
            kill();
            return;
        }
        _img = explosionSprites.getImage(currentFrame);
    }
}
