/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy03.cc: code for enemy 03

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Enemy06::Enemy06(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 48, drop)
{
    _score = 500;
    _dx = -Fix(moveTicks);
    updateImage(_stg.assets.enemy06->getImage(0));
    fireTicks = moveTicks;
    addFlags(SPRITE_NOSCROLL);
}

void Enemy06::doEnemyTick()
{
    damagePlayerOnTouch();
    _x += _dx / 10;
    if (moveTicks > 0)
    {
        _dx += 1_x;
        if (--moveTicks == 0)
        {
            _dx = 0_x;
            waitTicks = 30;
        }
    }
    if (_x >= S_WIDTH)
    {
        kill();
        return;
    }
    if (waitTicks && --waitTicks <= 0)
    {
        waitTicks = 0;
        moveTicks = 600;
    }
    if (--fireTicks <= 0)
    {
        int interval = 60;
        if (_stg.isPlayerAlive())
        {
            FireEnemyBullet(_stg, BulletType::Enemy6, _x - 2, _y + 12,
                Fix2D(-6_x, 0_x));
        }
        if (_dx > 0 && _stg.difficulty == DifficultyLevel::HARD)
            interval = 30;
        else if (_dx > 0 && _stg.difficulty == DifficultyLevel::BIZARRE)
            interval = 15;
        fireTicks += ScaleFireTicks(_stg, interval);
    }
}
