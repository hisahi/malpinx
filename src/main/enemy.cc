/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy.cc: base enemy code

#include <stdexcept>
#include "enemy.hh"

static Image flashBuffer{S_WIDTH, S_HEIGHT};

void EnemySprite::blit(Image &fb, int xoff, int yoff) const
{
    if (!_flash)
    {
        Sprite::blit(fb, xoff, yoff);
        return;
    }
    int w = _img->width(), h = _img->height();
    int ox = _x.round() + xoff, oy = _y.round() + yoff;
    _img->blitFast(flashBuffer, ox, oy, 0, 0, w, h);
    flashBuffer.addSolid(Color(_flash, _flash, _flash), ox, oy, w, h);
    flashBuffer.blit(fb, ox, oy, ox, oy, w, h);
};

EnemySprite::EnemySprite(Shooter &stg, int id, Fix x, Fix y, int hp)
    : Sprite(id, nullptr, x, y, SPRITE_COLLIDE_SPRITES, SpriteType::Enemy),
        _stg(stg), _hp(hp)
{
}

bool EnemySprite::damage(int dmg)
{
    if (_dead) return false;
    _hp -= dmg;
    if (_hp <= 0)
    {
        explode();
        return true;
    }
    gotDamage();
    return false;
}

void EnemySprite::explode()
{
    stg->addScore(_score);
    stg->explode(_x + _width / 2, _y + _height / 2, _esize, false);
    kill();
}

int ScaleFireTicks(Shooter &stg, int value)
{
    switch (stg.difficulty)
    {
    case DifficultyLevel::EASY:
        return (value * 6) / 4;
    case DifficultyLevel::NORMAL:
        return value;
    case DifficultyLevel::HARD:
        return value / 2;
    case DifficultyLevel::BIZARRE:
        return value / 3;
    }
    return value;
}
