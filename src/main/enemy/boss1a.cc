/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// boss1a.cc: code for stage 1 miniboss

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"
#include "sfx.hh"

Boss1a::Boss1a(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : BossSprite(stg, id, x, y, 2000, drop)
{
    _score = 2000;
    updateImage(_stg.assets.boss1a->getImage(0));
    _targetXScroll = _stg.scroll.x + 176_x;
    _dx = 0_x;
    switch (_stg.difficulty)
    {
    case DifficultyLevel::EASY:
        _dy = 0.5_x;
        break;
    case DifficultyLevel::NORMAL:
        _dy = 0.75_x;
        break;
    case DifficultyLevel::HARD:
        _dy = 1_x;
        break;
    case DifficultyLevel::BIZARRE:
        _dy = 1.5_x;
        break;
    }
}

void Boss1a::killEnemy()
{
    EnemySprite::killEnemy();
    if (_scrollStopped)
        std::swap(_stg.xSpeed, _oldXScroll);
}

void Boss1a::explode()
{
    _stg.explode(_x + _width / 2, _y + _height / 2,
                ExplosionSize::Large, false, false);
    _stg.explode(_x + _width / 2 - 40, _y + _height / 2,
                ExplosionSize::Large, true, false);
    _stg.explode(_x + _width / 2 + 40, _y + _height / 2,
                ExplosionSize::Large, true, false);
    killEnemy();
}

static const Fix ANGLE_OFFSET[5] = { 
    Fix::PI / 3, Fix::PI / 6, 0_x, Fix::PI / -6, Fix::PI / -3
};

static inline Fix getBoss1aAngle(int offset)
{
    offset = offset % 20;
    if (offset >= 10)
        offset = 19 - offset;
    if (offset >= 5)
        offset = 9 - offset;
    return ANGLE_OFFSET[offset];
}

void Boss1a::doBossTick()
{
    int max, mod;
    damagePlayerOnTouch();
    if (!_scrollStopped && _stg.scroll.x >= _targetXScroll)
    {
        _scrollStopped = true;
        std::swap(_stg.xSpeed, _oldXScroll);
        _stg.scroll.x = _targetXScroll;
    }
    switch (mode)
    {
    case 0:
        if (_stg.isPlayerAlive())
        {
            minY = std::max(0_x, _stg.player->y() - 64);
            maxY = std::min(Fix(S_GHEIGHT - 64), _stg.player->y() + 24);
        }
        if ((_y < minY && _dy < 0) || (_y > maxY && _dy > 0))
            _dy = -_dy;
        if (_stg.isPlayerAlive() && (modeTicks % 4) == 0)
        {
            if ((modeTicks % 20) == 0)
                vecPl = FixNorm(_stg.vecToPlayer(_x + 23, _y + 42), 3.5_x);
            if ((modeTicks % 40) >= 20)
                FireEnemyBullet(_stg, BulletType::Enemy4, _x + 23, _y + 42,
                    FixRotate(vecPl, getBoss1aAngle(modeTicks / 4)), true);
        }
        if (++modeTicks >= 160)
        {
            mode = 1;
            modeTicks = 0;
        }
        move();
        break;
    case 1:
        if (_stg.isPlayerAlive())
        {
            minY = std::max(0_x, _stg.player->y() - 48);
            maxY = std::min(Fix(S_HEIGHT - 64), _stg.player->y() - 8);
        }
        if ((_y < minY && _dy < 0) || (_y > maxY && _dy > 0))
            _dy = -_dy;
        move();
        if (++modeTicks >= ScaleFireTicks(_stg, 50))
        {
            mode = 2;
            modeTicks = 0;
        }
        break;
    case 2:
        max = ScaleFireTicks(_stg, 50);
        mod = max / 3;
        if ((modeTicks % mod) == (mod - 1))
        {
            PlaySound(SoundEffect::Boss1aBeam);
            FireEnemyBullet(_stg, BulletType::Boss1aBeam,
                    _x + 1, _y + 19, -6_x, 0_x, false);
            FireEnemyBullet(_stg, BulletType::Boss1aBeam,
                    _x - 15, _y + 27, -6_x, 0_x, false);
        }
        if (modeTicks >= mod)
        {
            if (_stg.isPlayerAlive())
            {
                minY = std::max(0_x, _stg.player->y() - 48);
                maxY = std::min(Fix(S_HEIGHT - 64), _stg.player->y() - 8);
                if ((_y < _stg.player->y() && _dy < 0)
                        || (_y > _stg.player->y() && _dy > 0))
                    _dy = -_dy;
            }
            if ((_y < minY && _dy < 0) || (_y > maxY && _dy > 0))
                _dy = -_dy;
            move();
        }
        if (++modeTicks >= max)
        {
            mode = 0;
            modeTicks = 0;
        }
    }   
}
