/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy14.cc: code for enemy 14

#include "enemy.hh"
#include "fix.hh"

static const int JUMP_HEIGHT[] = { 192, 160, 128, 96, 64, 32, 64, 64,
                                   192, 160, 128, 96, 64, 32, 64, 64 };

Enemy14::Enemy14(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 100, drop), origY(y)
{
    _score = 450;
    int xv = subtype >> 4;
    _dy = JUMP_HEIGHT[subtype & 15] / -20.5_x;
    ddy = -_dy / 40;
    _dx = 0.125_x * (((xv & 1) ? 1 : -1) * ((xv + 1) / 2));
    _stg.explode(_x + _width / 2, _y + _height - 4,
        ExplosionSize::WaterSplash, false, false);
}

void Enemy14::doEnemyTick()
{
    if (!(_ticks & 7))
        updateImage(_stg.assets.enemy14->getImage((_ticks >> 3) & 7));
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
