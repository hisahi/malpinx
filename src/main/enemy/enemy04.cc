/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy04.cc: code for enemy 04

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Enemy04::Enemy04(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 32, drop)
{
    _score = 300;
    fireTicks = ScaleFireTicks(stg,
            (((x.round() ^ (x.round() >> 8) * 37) + 15) % 90) >> 1);
}

void Enemy04::doEnemyTick()
{
    if (!(_ticks & 7))
        updateImage(_stg.assets.enemy04->getImage((_ticks >> 3) & 3));
    damagePlayerOnTouch();
    if (_x <= -_width)
    {
        kill();
        return;
    }
    if (--fireTicks <= 0)
    {
        if (_stg.isPlayerAlive())
        {
            Fix2D delta = FixNorm(_stg.vecToPlayer(_x + 8, _y + 8), 3_x);
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 4, _y + 4,
                    delta, false);
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 4, _y + 4,
                    FixRotate(delta, Fix::PI / 8), false);
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 4, _y + 4,
                    FixRotate(delta, -Fix::PI / 8), false);
        }
        fireTicks += ScaleFireTicks(_stg, 90);
    }
}
