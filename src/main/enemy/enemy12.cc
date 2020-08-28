/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy12.cc: code for enemy 12

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Enemy12::Enemy12(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 32, drop)
{
    _score = 450;
    fireTicks = ScaleFireTicks(stg,
            (((x.round() ^ (x.round() >> 8) * 37) + 15) % 24) >> 1);
    angleDiff = 1 - ((subtype & 1) << 1);
}

void Enemy12::doEnemyTick()
{
    if (!(_ticks & 7))
        updateImage(_stg.assets.enemy12->getImage((_ticks >> 3) & 3));
    damagePlayerOnTouch();
    if (_x <= -_width)
    {
        kill();
        return;
    }
    if (--fireTicks <= 0)
    {
        Fix2D vec = Fix2D(FixPolar2D(3_x, Fix::PI + (angle * Fix::PI / 8)));
        FireEnemyBullet(_stg, BulletType::Enemy4,
                _x + 4, _y + 4, vec, false, 2);
        angle = (angle + angleDiff) & 15;
        fireTicks += ScaleFireTicks(_stg, 30);
    }
}
