/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// bullet.cc: bullet object

#include <cmath>
#include "fix.hh"
#include "fixrng.hh"
#include "stage.hh"
#include "bullet.hh"
#include "explode.hh"
#include "sfx.hh"
#include "player.hh"

// bullet subframe simulation count
constexpr int BULLET_DIV = 2;
constexpr int BULLET_SEED = 883276465;

static int beamOffset = -1;
static bool crossDir = false;
static FixRandom suicideAngleRng(BULLET_SEED);

static constexpr int TRACK_FRAMES_COUNT = 16;
static const Fix TRACK_VEL = 6_x;
static const Fix TRACK_ANGLE_DIV = (0.5_x * TRACK_FRAMES_COUNT) / Fix::PI;

static inline SpriteType GetBulletSpriteType(BulletSource source)
{
    switch (source)
    {
    case BulletSource::Player:
        return SpriteType::BulletPlayer;
    case BulletSource::Enemy:
        return SpriteType::BulletEnemy;
    }
    return SpriteType::BulletEnemy;
}

BulletSprite::BulletSprite(Shooter &stg, int id, Fix x, Fix y, Fix dx, Fix dy,
                            BulletType type, BulletSource source)
    : Sprite(id, nullptr, x, y, SPRITE_NOSCROLL
        | SPRITE_COLLIDE_SPRITES | SPRITE_COLLIDE_FG,
        GetBulletSpriteType(source)), _type(type), _src(source), _vel(dx, dy),
        _expl(ExplosionSize::TinyWhite), _stg(stg), _damage(1), _pierce(false)
{
    int frameCount = 1;
    switch (type)
    {
        case BulletType::PulseDrone:
            _damage = 5;
            goto pulse;
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
            _damage = 9;
        spray:
            _img = stg.assets.bulletSprites->getImage(_frame = 1);
            _expl = ExplosionSize::TinyGreen;
            break;
        case BulletType::BeamDrone:
            _damage = 8;
            goto beam;
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
        case BulletType::TrackDrone:
            _damage = 5;
            goto track;
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
            _animSpeed = 0;
            frameCount = TRACK_FRAMES_COUNT;
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
        case BulletType::Enemy6:
            _img = stg.assets.bulletSprites->getImage(_frame = 28);
            frameCount = 4;
            _animSpeed = 4;
            _damage = 8;
            _expl = ExplosionSize::TinyYellow;
            break;
        case BulletType::Sigma:
            _img = stg.assets.sigma->getImage(_frame = 0);
            frameCount = 16;
            _animSpeed = 2;
            _damage = 20;
            _sigma = true;
            _vel = Fix2D();
            removeFlags(SPRITE_COLLIDE_FG);
            break;
        case BulletType::SuicideBullet:
            _img = stg.assets.bulletSprites->getImage(_frame = 26);
            frameCount = 2;
            _animSpeed = 4;
            _damage = 16;
            _expl = ExplosionSize::TinyWhite;
            break;
        case BulletType::Boss1aBeam:
            _img = stg.assets.bulletSprites->getImage(_frame = 32);
            _expl = ExplosionSize::TinyRed;
            _pierce = true;
            break;
    }
    if (source == BulletSource::Player)
        hitTargets.reserve(4);
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

static const Fix trackMaxTurnAngles[3] = {
    Fix::PI / 80, Fix::PI / 40, Fix::PI / 20
};

void BulletSprite::tickTrack(int trackLevel)
{
    Fix angle = FixPolar2D(_vel).angle;
    std::shared_ptr<Sprite> target = trackTarget.lock();
    if (!target) // try to pick target
    {
        bool newTarget = false;
        Fix minDistance{0};
        for (auto &s : _stg.spriteLayer2)
        {
            if (s->type() == SpriteType::Enemy)
            {
                Fix2D goal = Fix2D(s->x(), s->y()) + s->trackTarget();
                FixPolar2D dir = FixPolar2D(Fix2D(goal.x - _x, goal.y - _y));
                if (!minDistance || dir.length < minDistance)
                {
                    bool valid = true;
                    switch (trackLevel)
                    {
                    case 1:
                        valid &= SubtractAngles(dir.angle, angle).abs()
                                    < Fix::PI / 6;
                        break;
                    case 2:
                        valid &= SubtractAngles(dir.angle, angle).abs()
                                    < Fix::PI / 2;
                        break;
                    }
                    if (valid)
                    {
                        minDistance = dir.length;
                        target = s;
                        newTarget = true;
                    }
                }
            }
        }
        if (newTarget)
            trackTarget = std::weak_ptr<Sprite>(target);
    }
    if (target) // home in to target
    {
        Fix targetAngle = FixPolar2D(Fix2D(target->x() - _x,
                                           target->y() - _y)).angle;
        Fix angleDiff = SubtractAngles(targetAngle, angle);
        Fix maxAngle = trackMaxTurnAngles[trackLevel - 1];
        if (angleDiff > maxAngle)
            angleDiff = maxAngle;
        else if (angleDiff < -maxAngle)
            angleDiff = -maxAngle;
        angle += angleDiff;
        _vel = Fix2D(FixPolar2D(TRACK_VEL, angle));
        int spriteIndex = (angle * TRACK_ANGLE_DIV + 0.5_x).round()
                        % TRACK_FRAMES_COUNT;
        spriteIndex = (spriteIndex + TRACK_FRAMES_COUNT) % TRACK_FRAMES_COUNT;
        updateImageCentered(_stg.assets.bulletSprites->getImage(
                    _minFrame + spriteIndex));
    }
}

void BulletSprite::tick()
{
    ++_ticks;
    if (_animSpeed && _minFrame != _maxFrame && !(_ticks % _animSpeed))
    {
        ++_frame;
        if (_frame > _maxFrame)
        {
            if (_sigma)
            {
                kill();
                return;
            }
            else
                _frame = _minFrame;
        }
        if (_sigma)
            updateImage(_stg.assets.sigma->getImage(_frame));
        else
            updateImage(_stg.assets.bulletSprites->getImage(_frame));
    }

    int ox = S_WIDTH, oy = S_HEIGHT, nx, ny;
    Fix2D _fvel = _vel / BULLET_DIV;

    switch (_type)
    {
    case BulletType::Track1:
        tickTrack(1);
        break;
    case BulletType::Track2:
        tickTrack(2);
        break;
    case BulletType::Track3:
        tickTrack(3);
        break;
    }

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
                    hitTargets.push_back(s.get());

            if (hitTargets.size() > 1 && _vel)
            {
                // sort targets by distance
                Fix x = _x, y = _y, dx = _vel.x, dy = _vel.y;
                std::sort(hitTargets.begin(), hitTargets.end(),
                    [=](const Sprite *a, const Sprite *b)
                    {
                        return approxDistance(x, y, dx, dy, *a)
                             < approxDistance(x, y, dx, dy, *b);
                    });
            }

            for (Sprite *s : hitTargets)
            {
                if ((!s->damage(_damage) || !_pierce) && !_sigma)
                {
                    explode();
                    break;
                }
            }

            if (_sigma)
                for (auto &s : _stg.spriteLayer3)
                    if (s && s->type() == SpriteType::BulletEnemy && hits(*s))
                        s->kill();
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
    
    if (_x >= S_WIDTH || _y > _stg.stage->levelHeight || _y < -_height)
    {
        kill();
        return;
    }
}

void ResetBullets(int stageNum)
{
    suicideAngleRng.reset(BULLET_SEED + stageNum);
}

void SpawnPlayerBullet(Shooter &stg, Fix x, Fix y,
                        Fix dx, Fix dy, BulletType type)
{
    stg.spriteLayer3.push_back(std::make_shared<BulletSprite>(
        stg, stg.nextSpriteID(), x, y, dx, dy, type, BulletSource::Player));
}

void SpawnEnemyBullet(Shooter &stg, Fix x, Fix y,
                        Fix dx, Fix dy, BulletType type)
{
    stg.spriteLayer3.push_back(std::make_shared<BulletSprite>(
        stg, stg.nextSpriteID(), x, y, dx, dy, type, BulletSource::Enemy));
}

void FireSuicideBullet(Shooter &stg, Fix x, Fix y)
{
    Fix2D d(FixPolar2D(5_x, suicideAngleRng.nextAngle()));
    SpawnEnemyBullet(stg, x, y, d.x, d.y, BulletType::SuicideBullet);
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
            SpawnPlayerBullet(stg, x - 4, y - 3, 
                6_x, 0_x, BulletType::Spray2);
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
            SpawnPlayerBullet(stg, x - 4, y - 2, TRACK_VEL, 0_x,
                                BulletType::Track1);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 4, y + 6, TRACK_VEL, 0_x,
                                BulletType::Track2);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 4, y + 6, TRACK_VEL, 0_x,
                                BulletType::Track3);
            break;
        }
        return 6;
    }
    return 0;
}

int FireDroneWeapon(Shooter &stg, int weapon, Fix x, Fix y)
{
    switch (weapon)
    {
    case 0: // pulse
        SpawnPlayerBullet(stg, x - 4, y - 2,
                    8_x, 0_x, BulletType::PulseDrone);
        return 4;
    case 1: // spray
        SpawnPlayerBullet(stg, x - 4, y - 3, 
                    6_x, 0_x, BulletType::Spray1);
        return 5;
    case 2: // beam
        SpawnPlayerBullet(stg, x - 16, y - 3,
                    16_x, 0_x, BulletType::BeamDrone);
        return 6;
    case 3: // track
        SpawnPlayerBullet(stg, x - 4, y - 2,
                    TRACK_VEL, 0_x, BulletType::TrackDrone);
        return 6;
    }
    return 0;
}

int FireSigma(Shooter &stg, Fix x, Fix y)
{
    PlaySound(SoundEffect::FireSigma);
    SpawnPlayerBullet(stg, x - 2, y - 64, 0_x, 0_x, BulletType::Sigma);
    return 30;
}

void FireEnemyBullet(Shooter &stg, BulletType type, Fix x, Fix y,
            Fix dx, Fix dy, int scaleMode /*= 1*/)
{
    switch (scaleMode)
    {
    case 1:
        switch (stg.difficulty)
        {
        case DifficultyLevel::EASY:
            dx *= 0.625_x;
            dy *= 0.625_x;
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
