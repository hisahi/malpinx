/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy10.cc: code for enemy 10

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Enemy10::Enemy10(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 32, drop)
{
    _score = 200;
    _dx = -0.75_x;
    sineMul = ((subtype & 1) ? 1 : -1) * ((subtype + 1) / 2);
    fireTicks = ScaleFireTicks(stg,
            (((x.round() ^ (x.round() >> 8) * 37) + 15) % 90) >> 1);
}

void Enemy10::doEnemyTick()
{
    if (!(_ticks & 7))
        updateImage(_stg.assets.enemy10->getImage((_ticks >> 3) & 7));
    if (--fireTicks <= 0)
    {
        FireEnemyBullet(_stg, BulletType::Enemy4,
            _x + 4, _y + 4, Fix2D(-3.5_x, 0_x), true);
        fireTicks += ScaleFireTicks(_stg, 128);
    }
    damagePlayerOnTouch();
    _dy = 2_x * sineMul * deltaSineTable[(_ticks << 2) & 127];
    move();
}
