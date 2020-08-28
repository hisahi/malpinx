/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy13.cc: code for enemy 13

#include "enemy.hh"
#include "fix.hh"

static const int JUMP_HEIGHT[] = { 192, 160, 128, 96, 64, 32, 64, 64,
                                   192, 160, 128, 96, 64, 32, 64, 64 };

Enemy13::Enemy13(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 60, drop), origY(y)
{
    _score = 150;
    int xv = subtype >> 4;
    _dy = JUMP_HEIGHT[subtype & 15] / -20.5_x;
    ddy = -_dy / 40;
    _dx = 0.125_x * (((xv & 1) ? 1 : -1) * ((xv + 1) / 2));
    _stg.explode(_x + _width / 2, _y + _height - 4,
        ExplosionSize::WaterSplash, false, false);
}

void Enemy13::doEnemyTick()
{
    if (!(_ticks & 3))
        updateImage(_stg.assets.enemy13->getImage((_ticks >> 2) & 3));
    damagePlayerOnTouch();
    move();
    if (_y >= origY)
    {
        _stg.explode(_x + _width / 2, _y + _height - 4,
            ExplosionSize::WaterSplash, false, false);
        kill();
    }
    else
        _dy += ddy;
}
