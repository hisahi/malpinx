/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// boss1b.cc: code for stage 1 boss

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"
#include "sfx.hh"
#include "songs.hh"

Boss1b::Boss1b(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : BossSprite(stg, id, x, y, 5000, drop)
{
    _score = 5000;
    isLevelBoss();
    updateImage(_stg.assets.boss1b->getImage(0));
    _dx = 0_x;
    switch (_stg.difficulty)
    {
    case DifficultyLevel::EASY:
        _dy = -0.5_x;
        break;
    case DifficultyLevel::NORMAL:
        _dy = -0.75_x;
        break;
    case DifficultyLevel::HARD:
        _dy = -1.25_x;
        break;
    case DifficultyLevel::BIZARRE:
        _dy = -1.5_x;
        break;
    }
    spawnTicks = 60;
}

void Boss1b::killEnemy()
{
    EnemySprite::killEnemy();
}

void Boss1b::explode()
{
    StopSong();
    explodeTicks = 300;
    _invulnerable = true;
    _redShift = true;
}

void Boss1b::doBossTick()
{
    if (explodeTicks)
    {
        if (explodeTicks % 15 == 0)
        {
            Fix ex = _x - 32 + (explosionRng.nextFix() * (_width + 32));
            Fix ey = _y - 32 + (explosionRng.nextFix() * (_height + 32));
            _stg.explode(ex, ey, ExplosionSize::Medium2, false, false);
            if (_stg.difficulty == DifficultyLevel::BIZARRE)
            {
                FireSuicideBullet(_stg, ex, ey);
                FireSuicideBullet(_stg, ex, ey);
                FireSuicideBullet(_stg, ex, ey);
                FireSuicideBullet(_stg, ex, ey);
                FireSuicideBullet(_stg, ex, ey);
            }
        }
        if (!--explodeTicks)
        {
            bool bizarre = _stg.difficulty == DifficultyLevel::BIZARRE;
            _stg.explode(_x + _width / 2, _y + _height / 2,
                        ExplosionSize::Large, false, bizarre);
            _stg.explode(_x + _width / 2 - 40, _y + _height / 2 - 30,
                        ExplosionSize::Large, true, bizarre);
            _stg.explode(_x + _width / 2 + 40, _y + _height / 2 - 30,
                        ExplosionSize::Large, true, bizarre);
            _stg.explode(_x + _width / 2 - 40, _y + _height / 2 + 30,
                        ExplosionSize::Large, true, bizarre);
            _stg.explode(_x + _width / 2 + 40, _y + _height / 2 + 30,
                        ExplosionSize::Large, true, bizarre);
            _stg.flashScreen(Color(S_MAXCLR, S_MAXCLR, S_MAXCLR), 2);
            _stg.runScript(60, 1);
            killEnemy();
        }
        return;
    }

    int max;
    Fix mdx = 0_x;
    damagePlayerOnTouch();
    if (spawnTicks)
    {
        _stg.xSpeed = 0_x;
        mdx = -160_x / 60;
        _x += mdx;
        if (--spawnTicks > 45)
            return;
    }
    switch (mode)
    {
    case 0:
        if (_stg.isPlayerAlive())
        {
            minY = std::max(-64_x, _stg.player->y() - 128);
            maxY = std::min(Fix(S_GHEIGHT - 64), _stg.player->y() + 72);
        }
        if ((_y < minY && _dy < 0) || (_y > maxY && _dy > 0))
            _dy = -_dy;
        if (_stg.isPlayerAlive() && modeTicks % 8 == 0
            && (modeTicks % 80) < 40)
        {
            Fix qdx = mdx * 0.5_x;
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 64, _y + 9,
                -4_x + qdx, 0_x, false);
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 60, _y + 16,
                -4_x + qdx, 0_x, false);
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 56, _y + 23,
                -4_x + qdx, 0_x, false);
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 56, _y + 97,
                -4_x + qdx, 0_x, false);
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 60, _y + 104,
                -4_x + qdx, 0_x, false);
            FireEnemyBullet(_stg, BulletType::Enemy4, _x + 64, _y + 111,
                -4_x + qdx, 0_x, false);
        }
        if (++modeTicks >= 240)
        {
            mode = 1;
            modeTicks = 0;
        }
        move();
        break;
    case 1:
        if (_stg.isPlayerAlive())
        {
            minY = std::max(0_x, _stg.player->y() - 96);
            maxY = std::min(Fix(S_GHEIGHT - 128), _stg.player->y() + 32);
        }
        if ((_y < minY && _dy < 0) || (_y > maxY && _dy > 0))
            _dy = -_dy;
        move();
        if (++modeTicks >= ScaleFireTicks(_stg, 80))
        {
            mode = 2;
            modeTicks = 0;
        }
        break;
    case 2:
        max = ScaleFireTicks(_stg, 60);
        if (modeTicks == 0)
        {
            PlaySound(SoundEffect::Boss1bRingFire);
            switch (_stg.difficulty)
            {
            case DifficultyLevel::EASY:
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -168_x / 60_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -112_x / 60_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -56_x / 60_x, 0_x, false, 0);
                break;
            case DifficultyLevel::NORMAL:
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -168_x / 45_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -112_x / 45_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -56_x / 45_x, 0_x, false, 0);
                break;
            case DifficultyLevel::HARD:
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -176_x / 30_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -132_x / 30_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -88_x / 30_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -44_x / 30_x, 0_x, false, 0);
                break;
            case DifficultyLevel::BIZARRE:
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -176_x / 24_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -132_x / 24_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -88_x / 24_x, 0_x, false, 0);
                FireEnemyBullet(_stg, BulletType::Boss1bRing, _x, _y + 48,
                    -44_x / 24_x, 0_x, false, 0);
                break;
            }
        }
        if (++modeTicks >= max)
        {
            mode = 0;
            modeTicks = 0;
        }
    }   
}
