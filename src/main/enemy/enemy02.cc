/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy02.cc: code for enemy 02

#include "enemy.hh"
#include "fix.hh"

Enemy02::Enemy02(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 20, drop)
{
    _score = 150;
    _dx = -1_x;
    yflip = (subtype & 1) != 0;
}

void Enemy02::doEnemyTick()
{
    if (!(_ticks & 7))
        updateImage(_stg.assets.enemy02->getImage((_ticks >> 3) & 3));
    damagePlayerOnTouch();
    move();
    _x -= 4_x * deltaSineTable[_ticks & 127];
    _y -= (yflip ? -4_x : 4_x) * deltaSineTable[(_ticks + 16) & 127];
}
