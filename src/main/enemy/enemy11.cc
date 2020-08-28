/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy11.cc: code for enemy 11

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Enemy11::Enemy11(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 64, drop)
{
    _score = 900;
    _dx = -1_x;
    _dy = 0_x;
    updateImage(_stg.assets.enemy11->getImage(0));
    fireTicks = ScaleFireTicks(stg,
            (((x.round() ^ (x.round() >> 8) * 37) + 15) % 90) >> 1);
}

void Enemy11::doEnemyTick()
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
                _x + 5, _y + 27, delta, false);
        }
        fireTicks += ScaleFireTicks(_stg, 128);
    }
}
