/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// bullet.cc: bullet object

#include <cmath>
#include "fix.hh"
#include "stage.hh"
#include "bullet.hh"
#include "explode.hh"
#include "sfx.hh"
#include "player.hh"

// bullet subframe simulation count
constexpr int BULLET_DIV = 2;

static int beamOffset = -1;
static bool crossDir = false;

BulletSprite::BulletSprite(Shooter &stg, int id, Fix x, Fix y, Fix dx, Fix dy,
                            BulletType type, BulletSource source)
    : Sprite(id, nullptr, x, y, SPRITE_NOSCROLL
        | SPRITE_COLLIDE_SPRITES | SPRITE_COLLIDE_FG,
        SpriteType::Bullet), _type(type), _src(source), _vel(dx, dy),
        _expl(ExplosionSize::TinyWhite), _stg(stg), _damage(1), _pierce(false)
{
    int frameCount = 1;
    switch (type)
    {
        case BulletType::Pulse1:
            _damage = 10;
            goto pulse;
        case BulletType::Pulse2:
            _damage = 6;
            goto pulse;
        case BulletType::Pulse3:
            _damage = 8;
        pulse:
            _img = stg.assets.bulletSprites->getImage(_frame = 0);
            _expl = ExplosionSize::TinyRed;
            break;
        case BulletType::Spray1:
            _damage = 8;
            goto spray;
        case BulletType::Spray2:
            _damage = 9;
            goto spray;
        case BulletType::Spray3:
            _damage = 10;
        spray:
            _img = stg.assets.bulletSprites->getImage(_frame = 1);
            _expl = ExplosionSize::TinyGreen;
            break;
        case BulletType::Beam1:
            _damage = 14;
            goto beam;
        case BulletType::Beam2:
            _damage = 16;
            goto beam;
        case BulletType::Beam3:
            _damage = 18;
        beam:
            _img = stg.assets.bulletSprites->getImage(_frame = 2);
            _expl = ExplosionSize::TinyBlue;
            _pierce = true;
            break;
        case BulletType::Track1:
            _damage = 6;
            goto track;
        case BulletType::Track2:
            _damage = 9;
            goto track;
        case BulletType::Track3:
            _damage = 12;
        track:
            _img = stg.assets.bulletSprites->getImage(_frame = 3);
            _expl = ExplosionSize::TinyPurple;
            break;
        case BulletType::Enemy3:
            _img = stg.assets.bulletSprites->getImage(_frame = 19);
            frameCount = 4;
            _animSpeed = 3;
            _damage = 11;
            _expl = ExplosionSize::TinyBlue;
            break;
        case BulletType::Enemy4:
            _img = stg.assets.bulletSprites->getImage(_frame = 23);
            frameCount = 3;
            _animSpeed = 4;
            _damage = 8;
            _expl = ExplosionSize::TinyYellow;
            break;
    }
    if (source == BulletSource::Player)
        hitTargets.reserve(16);
    _minFrame = _frame;
    _maxFrame = _minFrame + frameCount - 1;
    if (_img)
    {
        _width = _img->width();
        _height = _img->height();
    } else
        kill();
}

void BulletSprite::explode()
{
    stg->explode(_x + (_img->width() / 2), _y + (_img->height() / 2),
                _expl, true);
    kill();
}

static inline float euclideanDistance(Fix x1, Fix y1, Fix x2, Fix y2)
{
    float fx = std::fabs(static_cast<float>(x2 - x1));
    float fy = std::fabs(static_cast<float>(y2 - y1));
    return fx * fx + fy * fy;
}

static inline float approxDistance(Fix x, Fix y, Fix dx, Fix dy,
                                    const Sprite &s)
{
    Fix sx = s.x(), sy = s.y();
    Fix da = (dx.abs() + dy.abs()) / 2;
    int avg = (s.width() + s.height()) / 2;
    Fix scale = avg / da;
    if (dx == 0 && dy == 0) return 0;
    return euclideanDistance(x, y, sx + s.width() / 2 - scale * dx,
                                   sy + s.height() / 2 - scale * dy);
}

void BulletSprite::tick()
{
    ++_ticks;
    if (_x >= S_WIDTH || _y > _stg.stage->levelHeight || _y < -_height)
    {
        kill();
        return;
    }

    if (_minFrame != _maxFrame && !(_ticks % _animSpeed))
    {
        ++_frame;
        if (_frame > _maxFrame)
            _frame = _minFrame;
        updateImage(_stg.assets.bulletSprites->getImage(_frame));
    }

    int ox = S_WIDTH, oy = S_HEIGHT, nx, ny;
    Fix2D _fvel = _vel / BULLET_DIV;

    for (int i = 0; i < BULLET_DIV; ++i)
    {
        _x += _fvel.x;
        _y += _fvel.y;

        nx = static_cast<int>(_x);
        ny = static_cast<int>(_y);

        if (nx == ox && ny == oy)
            continue;
        ox = nx;
        oy = ny;

        if (this->hasFlag(SPRITE_COLLIDE_FG))
            for (auto &layer : _stg.stage->terrainLayers)
                if (layer->hitsSprite(*_img, _stg.scroll, _x, _y))
                {
                    explode();
                    return;
                }

        if (_src == BulletSource::Player)
        {
            hitTargets.clear();
            // check for enemies
            for (auto &s : _stg.spriteLayer2)
                if (s->type() == SpriteType::Enemy && hits(*s))
                    hitTargets.push_back(*s);

            if (hitTargets.size() > 1)
            {
                // sort targets by distance
                Fix x = _x, y = _y, dx = _vel.x, dy = _vel.y;
                std::sort(hitTargets.begin(), hitTargets.end(),
                    [=](const Sprite &a, const Sprite &b)
                    {
                        return approxDistance(x, y, dx, dy, a)
                             - approxDistance(x, y, dx, dy, b);
                    });
            }

            for (Sprite &s : hitTargets)
            {
                if (!s.damage(_damage) || !_pierce)
                {
                    explode();
                    break;
                }
            }
        }
        else if (_src == BulletSource::Enemy)
        {
            if (hits(_stg.player.get()))
            {
                if (!_stg.player->damage(_damage) || !_pierce)
                    explode();
            }
        }
    }
}

void SpawnPlayerBullet(Shooter &stg, Fix x, Fix y,
                        Fix dx, Fix dy, BulletType type)
{
    stg.spriteLayer3.push_back(std::make_unique<BulletSprite>(
        stg, stg.nextSpriteID(), x, y, dx, dy, type, BulletSource::Player));
}

void SpawnEnemyBullet(Shooter &stg, Fix x, Fix y,
                        Fix dx, Fix dy, BulletType type)
{
    stg.spriteLayer3.push_back(std::make_unique<BulletSprite>(
        stg, stg.nextSpriteID(), x, y, dx, dy, type, BulletSource::Enemy));
}

int FireWeapon(Shooter &stg, int weapon, int level, Fix x, Fix y)
{
    switch (weapon)
    {
    case 0: // pulse
        PlayGunSound(SoundEffect::FirePulse);
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 4, y - 2,
                        8_x, 0_x, BulletType::Pulse1);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 12, y - 8, 
                        8_x, 0_x, BulletType::Pulse2);
            SpawnPlayerBullet(stg, x - 12, y + 6,
                        8_x, 0_x, BulletType::Pulse2);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 12, y - 8,
                        10_x, 0_x, BulletType::Pulse3);
            SpawnPlayerBullet(stg, x - 12, y + 6,
                        10_x, 0_x, BulletType::Pulse3);
            break;
        }
        return 4;
    case 1: // spray
        PlayGunSound(SoundEffect::FireSpray);
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 4, y - 3, 
                6_x, 0_x, BulletType::Spray1);
            SpawnPlayerBullet(stg, x - 2, y - 6, 
                3_x, -6_x * 13 / 15, BulletType::Spray1);
            SpawnPlayerBullet(stg, x - 2, y, 
                3_x, 6_x * 13 / 15, BulletType::Spray1);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 3, y - 4, 
                6_x * 13 / 15, -3_x, BulletType::Spray2);
            SpawnPlayerBullet(stg, x - 3, y - 2, 
                6_x * 13 / 15, 3_x, BulletType::Spray2);
            SpawnPlayerBullet(stg, x - 36, y - 3, 
                -6_x, 0_x, BulletType::Spray2);
            SpawnPlayerBullet(stg, x - 3, y - 4, 
                6_x * 13 / 15, -3_x, BulletType::Spray2);
            SpawnPlayerBullet(stg, x - 3, y - 2, 
                6_x * 13 / 15, 3_x, BulletType::Spray2);
            SpawnPlayerBullet(stg, x - 36, y - 3, 
                -6_x, 0_x, BulletType::Spray2);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 3, y - 4, 
                6_x * 13 / 15, -3_x, BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 3, y - 2, 
                6_x * 13 / 15, 3_x, BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 4, y - 3, 
                6_x, 0_x, BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 2, y - 6, 
                3-x, -6_x * 13 / 15, BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 2, y, 
                3_x, 6_x * 13 / 15, BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 36, y - 4, 
                -3.5_x, -0.7_x, BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 36,  y - 2, 
                -3.5_x, 0.7_x, BulletType::Spray3);
            break;
        }
        return 5;
    case 2: // beam
        PlayGunSound(SoundEffect::FireBeam);
        beamOffset = -beamOffset;
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 16, y - 3, 16_x, 0_x, BulletType::Beam1);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 16, y - 3 + 5 * -beamOffset,
                16_x, 0_x, BulletType::Beam2);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 16, y - 3 + 5 * -beamOffset,
                16_x, 0_x, BulletType::Beam3);
            break;
        }
        return 6;
    case 3: // track
        PlayGunSound(SoundEffect::FireTrack);
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 4, y - 2, 6_x, 0_x, BulletType::Track1);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 4, y - 10, 6_x, 0_x, BulletType::Track2);
            SpawnPlayerBullet(stg, x - 4, y + 6, 6_x, 0_x, BulletType::Track2);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 4, y - 10, 6_x, 0_x, BulletType::Track3);
            SpawnPlayerBullet(stg, x - 4, y + 6, 6_x, 0_x, BulletType::Track3);
            break;
        }
        return 6;
    }
    return 0;
}

void FireEnemyBullet(Shooter &stg, BulletType type, Fix x, Fix y,
            Fix dx, Fix dy, bool scale /*= true*/)
{
    if (scale)
    {
        switch (stg.difficulty)
        {
        case DifficultyLevel::EASY:
            dx *= 0.75_x;
            dy *= 0.75_x;
            break;
        case DifficultyLevel::NORMAL:
            break;
        case DifficultyLevel::HARD:
        case DifficultyLevel::BIZARRE:
            dx *= 1.5_x;
            dy *= 1.5_x;
            break;
        }
    }
    SpawnEnemyBullet(stg, x, y, dx, dy, type);
}
