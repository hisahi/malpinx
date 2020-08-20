/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy.cc: base enemy code

#include <stdexcept>
#include "enemy.hh"
#include "bullet.hh"

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

EnemySprite::EnemySprite(Shooter &stg, int id, Fix x, Fix y,
        int hp, PowerupType drop)
    : Sprite(id, nullptr, x, y, SPRITE_COLLIDE_SPRITES, SpriteType::Enemy),
        _stg(stg), _hp(hp), _drop(drop)
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

void EnemySprite::killEnemy()
{
    _stg.addScore(_score);
    kill();
    if (_stg.difficulty == DifficultyLevel::BIZARRE)
        for (int i = 0; i < 3; ++i)
            FireSuicideBullet(_stg, _x + _width / 2, _y + _height / 2);
    if (_drop != PowerupType::None)
        _stg.spawnPowerup(_x + _width / 2, _y + _height / 2, _drop);
}

void EnemySprite::explode()
{
    _stg.explode(_x + _width / 2, _y + _height / 2, _esize, false, false);
    killEnemy();
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
        return (value * 2) / 3;
    case DifficultyLevel::BIZARRE:
        return value / 2;
    }
    return value;
}
