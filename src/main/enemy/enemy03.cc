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

Enemy03::Enemy03(Shooter &stg, int id, Fix x, Fix y, int subtype)
    : EnemySprite(stg, id, x, y, 64)
{
    _score = 400;
    _dx = -1_x;
    switch (subtype)
    {
    case 0:
        finalY = stg.player ? stg.player->y() - 8
                    : stg.scroll.y + (S_HEIGHT) / 2 - 16;
        break;
    case 1:
        finalY = 16_x;
        break;
    case 2:
        finalY = 64_x;
        break;
    case 3:
        finalY = 112_x;
        break;
    case 4:
        finalY = 160_x;
        break;
    }
    _dx = stg.xSpeed / 2;
    if (y > finalY)
        _dy = -1_x;
    else
        _dy = 1_x;
    updateImage(_stg.assets.enemy03->getImage(0));
    fireTicks = ScaleFireTicks(stg,
            ((x.round() ^ (x.round() >> 8) + 79) * 37) & 127);
}

void Enemy03::doEnemyTick()
{
    damagePlayerOnTouch();
    if (_dy < 0 && _y <= finalY)
        _dy = 0_x, _dx = -0.5_x;
    else if (_dy > 0 && _y >= finalY)
        _dy = 0_x, _dx = -0.5_x;
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
            Fix2D delta = FixNorm(_stg.vecToPlayer(_x, _y), 4_x);
            FireEnemyBullet(_stg, BulletType::Enemy3, _x + 5, _y + 27, delta);
        }
        fireTicks += ScaleFireTicks(_stg, 128);
    }
}
