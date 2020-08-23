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

static Image flashBuffer{S_WIDTH * 2, S_HEIGHT * 2};

void EnemySprite::blit(Image &fb, int xoff, int yoff) const
{
    if (!_flash && !_redShift)
    {
        Sprite::blit(fb, xoff, yoff);
        return;
    }
    int w = _img->width(), h = _img->height();
    int ox = _x.round() + xoff, oy = _y.round() + yoff,
        tx = ox + S_WIDTH, ty = oy + S_HEIGHT;
    _img->blitFast(flashBuffer, tx, ty, 0, 0, w, h);
    flashBuffer.addSolid(Color(_flash, _flash, _flash), tx, ty, w, h);
    if (_redShift)
        flashBuffer.subtractSolid(Color(0, S_MAXCLR, S_MAXCLR), tx, ty, w, h);
    flashBuffer.blit(fb, ox, oy, tx, ty, w, h);
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
    if (_invulnerable)
    {
        gotNoDamage();
        return false;
    }
    _hp -= dmg;
    if (_hp <= 0)
    {
        killEnemyByPlayer();
        return true;
    }
    gotDamage();
    return false;
}

void EnemySprite::killEnemyByPlayer()
{
    _stg.addScore(_score);
    explode();
    if (_drop != PowerupType::None)
        _stg.spawnPowerup(_x + _width / 2, _y + _height / 2, _drop);
}

void EnemySprite::killEnemy()
{
    kill();
    if (_stg.difficulty == DifficultyLevel::BIZARRE)
        for (int i = 0; i < 5; ++i)
            FireSuicideBullet(_stg, _x + _width / 2, _y + _height / 2);
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
