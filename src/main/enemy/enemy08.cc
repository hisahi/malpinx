/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy08.cc: code for enemy 08

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Enemy08::Enemy08(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 10, drop)
{
    switch (subtype)
    {
    case 0:
        yflip = false;
        updateImage(_stg.assets.enemy08->getImage(4));
        trackAngle = pointAngle = Fix::PI / 2;
        break;
    case 1:
        yflip = true;
        updateImage(_stg.assets.enemy08->getImage(13));
        trackAngle = pointAngle = 3 * Fix::PI / 2;
        break;
    default:
        kill();
        break;
    }
    switch (stg.difficulty)
    {
    case DifficultyLevel::EASY:
        turnSpeed = Fix::PI / S_TICKS;
        break;
    case DifficultyLevel::NORMAL:
        turnSpeed = Fix::PI * 3 / (2 * S_TICKS);
        break;
    case DifficultyLevel::HARD:
    case DifficultyLevel::BIZARRE:
        turnSpeed = Fix::PI * 2 / S_TICKS;
        break;
    }
    _score = 300;
    fireTicks = ScaleFireTicks(stg, 40);
}

void Enemy08::doEnemyTick()
{
    damagePlayerOnTouch();
    if (_x <= -_width)
    {
        kill();
        return;
    }
    if (_stg.isPlayerAlive())
    {
        Fix2D delta = _stg.vecToPlayer(_x + 8, _y + 8);
        if (delta)
        {
            Fix deltaAngle = FixPolar2D(delta).angle;
            if (deltaAngle < 0)
                deltaAngle += Fix::TAU;
            int adjTicks = ScaleFireTicks(_stg, 15);
            int maxTicks = ScaleFireTicks(_stg, 60);
            Fix diffAngle = SubtractAngles(deltaAngle, trackAngle);
            if (diffAngle.abs() > turnSpeed)
                diffAngle = diffAngle.sign() * turnSpeed;
            if (diffAngle != 0)
            {
                Fix oldPointAngle = pointAngle;
                if (yflip)
                    trackAngle = clamp(trackAngle + diffAngle,
                                    Fix::PI, Fix::TAU);
                else
                    trackAngle = clamp(trackAngle + diffAngle, 0_x, Fix::PI);

                Fix spriteAngle = yflip ? trackAngle - Fix::PI : trackAngle;
                spriteAngle += Fix::PI / 16;
                int index = (spriteAngle / (Fix::PI / 8)).round();
                if (yflip)
                {
                    pointAngle = (8 + index) * (Fix::PI / 8);
                    index += 9;
                }
                else
                {
                    pointAngle = index * (Fix::PI / 8);
                    index = 8 - index;   
                }

                if (oldPointAngle != pointAngle)
                {
                    updateImage(_stg.assets.enemy08->getImage(index));
                    fireTicks = std::min(maxTicks, fireTicks
                        + (adjTicks * (pointAngle - oldPointAngle).abs()
                            .round()));
                }
            }
        }
        if (--fireTicks <= 0)
        {
            Fix2D o = Fix2D(FixPolar2D(8_x, pointAngle));
            FireEnemyBullet(_stg, BulletType::Enemy4,
                _x + 8 + o.x, _y + 8 + o.y, o * (3_x / 8), true);
            fireTicks += ScaleFireTicks(_stg, 60);
        }
    }
}
