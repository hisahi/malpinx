/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy05.cc: code for enemy 05

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Enemy07::Enemy07(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 80, drop)
{
    _score = 300;
    _dx = -1.5_x, _dy = -1_x;
    addFlags(SPRITE_NOSCROLL);
    switch (stg.difficulty)
    {
    case DifficultyLevel::EASY:
        break;
    case DifficultyLevel::NORMAL:
        _hp = 50;
        break;
    case DifficultyLevel::HARD:
    case DifficultyLevel::BIZARRE:
        _hp = 20;
        break;
    }
}

void Enemy07::explode()
{
    _stg.explode(_x + 4, _y + 4, ExplosionSize::LargeBlue,
        false, true);
    killEnemy();
}

void Enemy07::doEnemyTick()
{
    if (!(_ticks & 3))
        updateImage(_stg.assets.enemy07->getImage((_ticks >> 2) & 3));
    damagePlayerOnTouch();
    move();
    if (_y < -_height - 8)
        kill();
}
