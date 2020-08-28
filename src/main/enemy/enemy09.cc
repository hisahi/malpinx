/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy09.cc: code for enemy 09

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Enemy09::Enemy09(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 50, drop)
{
    _score = 400;
    _dx = -0.375_x;
    updateImage(_stg.assets.enemy09->getImage(0));
    fireTicks = ScaleFireTicks(stg,
            (((x.round() ^ (x.round() >> 8) * 37) + 15) % 50) >> 1);
}

void Enemy09::doEnemyTick()
{
    damagePlayerOnTouch();
    move();
    if (_x <= -_width)
    {
        kill();
        return;
    }
    if (--fireTicks <= 0)
    {
        if (_stg.isPlayerAlive())
        {
            Fix2D delta = FixNorm(_stg.vecToPlayer(_x + 5, _y + 27), 4_x);
            FireEnemyBullet(_stg, BulletType::Enemy3,
                _x + 4, _y + 4, delta, false);
        }
        fireTicks += ScaleFireTicks(_stg, 80);
    }
}
