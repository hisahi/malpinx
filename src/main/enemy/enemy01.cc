/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy01.cc: code for enemy 01

#include "enemy.hh"
#include "fix.hh"

Enemy01::Enemy01(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 10, drop)
{
    _score = 100;
    _dx = -1.5_x;
    sineMul = ((subtype & 1) ? 1 : -1) * ((subtype + 1) / 2);
}

void Enemy01::doEnemyTick()
{
    if (!(_ticks & 7))
        updateImage(_stg.assets.enemy01->getImage((_ticks >> 3) & 7));
    damagePlayerOnTouch();
    _dy = 2_x * sineMul * deltaSineTable[(_ticks << 2) & 127];
    move();
}
