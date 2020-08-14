/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy05.cc: code for enemy 05

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

static const Fix SPEED = 4_x;

Enemy05::Enemy05(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : EnemySprite(stg, id, x, y, 48, drop), moveTicks(30), speed(1)
{
    _score = 300;
    addFlags(SPRITE_NOSCROLL);
    switch (subtype)
    {
    case 0:
        _dx = -SPEED;
        break;
    case 1:
        _dy = SPEED;
        moveTicks += 30;
        break;
    case 2:
        _dy = -SPEED;
        moveTicks += 30;
        break;
    }
}

void Enemy05::doEnemyTick()
{
    if (!(_ticks & 7))
        updateImage(_stg.assets.enemy05->getImage((_ticks >> 3) & 3));
    damagePlayerOnTouch();
    if (_x <= -_width || (!moveTicks && (_x >= S_WIDTH
                        || _y >= S_HEIGHT || _y < -_height)))
    {
        kill();
        return;
    }
    if (speed > 0)
        speed -= 1_x / 30_x;
    else if (speed < 0)
        speed = 0_x;
    if (--moveTicks <= 0 && _stg.isPlayerAlive())
    {
        Fix2D vel = FixNorm(_stg.vecToPlayer(_x + 8, _y + 8), 5_x);
        _dx = vel.x;
        _dy = vel.y;
        speed = 1_x;

        switch (_stg.difficulty)
        {
        case DifficultyLevel::EASY:
            moveTicks = 90;
            break;
        case DifficultyLevel::NORMAL:
            moveTicks = 60;
            break;
        case DifficultyLevel::HARD:
        case DifficultyLevel::BIZARRE:
            moveTicks = 45;
            break;
        }
    }
    _x += _dx * speed;
    _y += _dy * speed;
}
