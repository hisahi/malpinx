/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// bullet.cc: bullet object

#include <random>
#include "fix.hh"
#include "stage.hh"
#include "bullet.hh"
#include "explode.hh"
#include "sfx.hh"
#include "player.hh"

static int beamOffset = -1;
static bool crossDir = false;

BulletSprite::BulletSprite(Shooter &stg, int id, int x, int y, Fix dx, Fix dy,
                            BulletType type, BulletSource source)
    : Sprite(id, nullptr, x, y, SPRITE_NOSCROLL
        | SPRITE_COLLIDE_SPRITES | SPRITE_COLLIDE_FG,
        SpriteType::Bullet), _type(type), _src(source),
      _fx(x * Fix::ONE + Fix::HALF), _fy(y * Fix::ONE + Fix::HALF), _damage(1),
      _dx(dx), _dy(dy), _height(16), _stg(stg), _expl(ExplosionSize::TinyWhite)
{
    switch (type)
    {
        case BulletType::Pulse12:
        case BulletType::Pulse3:
            _img = stg.assets.bulletSprites->getImage(0);
            break;
        case BulletType::Spray12:
        case BulletType::Spray3:
            _img = stg.assets.bulletSprites->getImage(1);
            break;
        case BulletType::CrossNW12:
        case BulletType::CrossNW3:
            _img = stg.assets.bulletSprites->getImage(2);
            break;
        case BulletType::CrossNE12:
        case BulletType::CrossNE3:
            _img = stg.assets.bulletSprites->getImage(3);
            break;
        case BulletType::CrossSW12:
        case BulletType::CrossSW3:
            _img = stg.assets.bulletSprites->getImage(4);
            break;
        case BulletType::CrossSE12:
        case BulletType::CrossSE3:
            _img = stg.assets.bulletSprites->getImage(5);
            break;
        case BulletType::Beam1:
        case BulletType::Beam2:
        case BulletType::Beam3:
            _img = stg.assets.bulletSprites->getImage(6);
            break;
        case BulletType::FlakUp:
        case BulletType::FlakUp3:
            _img = stg.assets.bulletSprites->getImage(7);
            break;
        case BulletType::FlakDown:
        case BulletType::FlakDown3:
            _img = stg.assets.bulletSprites->getImage(8);
            break;
        case BulletType::FlakForward:
        case BulletType::FlakForward3:
            _img = stg.assets.bulletSprites->getImage(9);
            break;
        case BulletType::Track1:
        case BulletType::Track2:
        case BulletType::Track3:
            _img = stg.assets.bulletSprites->getImage(10);
            break;
    }
    if (source == BulletSource::Player)
        hitTargets.reserve(16);
    if (_img)
    {
        _width = _img->width();
        _height = _img->height();
    }
}

void BulletSprite::explode()
{
    stg->explode(_x + _img->width() / 2,
                _y + _img->height() / 2,
                _expl, true);
    kill();
}

void BulletSprite::tick()
{
    ++_ticks;

    _fx += _dx;
    _fy += _dy;
    _x = static_cast<int>(_fx);
    _y = static_cast<int>(_fy);
    if (_x >= S_WIDTH || _y > _stg.stage->levelHeight || _y < -_height)
    {
        kill();
        return;
    }

    if (_ticks % 3 == 0)
        switch (_type)
        {
        case BulletType::FlakUp:
        case BulletType::FlakUp3:
        case BulletType::FlakDown:
            _damage -= 1;
            _dy += Fix::ONE;
            if (!_dy)
            {
                kill();
                return;
            }
            break;
        case BulletType::FlakDown3:
            _damage -= 1;
            _dy -= Fix::ONE;
            if (!_dy)
            {
                kill();
                return;
            }
            break;
        case BulletType::FlakForward:
        case BulletType::FlakForward3:
            _damage -= 1;
            _dx -= Fix::ONE;
            if (!_dx)
            {
                kill();
                return;
            }
            break;
        }

    if (this->hasFlag(SPRITE_COLLIDE_FG))
        for (auto &layer : _stg.stage->terrainLayers)
            if (layer->hitsSprite(*_img, _stg.scroll, _x, _y))
            {
                explode();
                return;
            }

    if (_src == BulletSource::Player)
    {
        // check for enemies
        hitTargets.clear();

    }
    if (_src == BulletSource::Enemy)
    {
        // check for player collision
        if (hits(_stg.player))
        {
            _stg.player->damage(_damage);
            explode();
        }
    }
}

void SpawnPlayerBullet(Shooter &stg, int x, int y,
                        Fix dx, Fix dy, BulletType type)
{
    stg.spriteLayer3.push_back(std::make_unique<BulletSprite>(
                stg, stg.nextSpriteID(), x, y, dx, dy, type,
                            BulletSource::Player));
}

int FireWeapon(Shooter &stg, int weapon, int level, int x, int y)
{
    switch (weapon)
    {
    case 0: // pulse
        PlayGunSound(SoundEffect::FirePulse);
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 4, y - 2, 
                Fix(8 * Fix::ONE), 0, BulletType::Pulse12);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 12, y - 8, 
                Fix(8 * Fix::ONE), 0, BulletType::Pulse12);
            SpawnPlayerBullet(stg, x - 12, y + 6, 
                Fix(8 * Fix::ONE), 0, BulletType::Pulse12);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 12, y - 8, 
                Fix(8 * Fix::ONE), 0, BulletType::Pulse3);
            SpawnPlayerBullet(stg, x - 12, y + 6, 
                Fix(8 * Fix::ONE), 0, BulletType::Pulse3);
            break;
        }
        return 4;
    case 1: // spray
        PlayGunSound(SoundEffect::FireSpray);
        switch (level)
        {
        case 1:
            SpawnPlayerBullet(stg, x - 3, y - 4, 
                Fix(6 * 13 * Fix::ONE / 15), Fix(-3 * Fix::ONE),
                BulletType::Spray12);
            SpawnPlayerBullet(stg, x - 3, y - 2, 
                Fix(6 * 13 * Fix::ONE / 15), Fix(3 * Fix::ONE),
                BulletType::Spray12);
        case 0:
            SpawnPlayerBullet(stg, x - 4, y - 3, 
                Fix(6 * Fix::ONE), 0, BulletType::Spray12);
            SpawnPlayerBullet(stg, x - 2, y - 6, 
                Fix(3 * Fix::ONE), Fix(-6 * 13 * Fix::ONE / 15),
                BulletType::Spray12);
            SpawnPlayerBullet(stg, x - 2, y, 
                Fix(3 * Fix::ONE), Fix(6 * 13 * Fix::ONE / 15),
                BulletType::Spray12);
            SpawnPlayerBullet(stg, x - 36, y - 3, 
                Fix(-6 * Fix::ONE), 0, BulletType::Spray12);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 3, y - 4, 
                Fix(6 * 13 * Fix::ONE / 15), Fix(-3 * Fix::ONE),
                BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 3, y - 2, 
                Fix(6 * 13 * Fix::ONE / 15), Fix(3 * Fix::ONE),
                BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 4, y - 3, 
                Fix(6 * Fix::ONE), 0, BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 2, y - 6, 
                Fix(3 * Fix::ONE), Fix(-6 * 13 * Fix::ONE / 15),
                BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 2, y, 
                Fix(3 * Fix::ONE), Fix(6 * 13 * Fix::ONE / 15),
                BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 36, y - 4, 
                Fix(-5 * Fix::ONE), Fix(-1 * Fix::ONE), BulletType::Spray3);
            SpawnPlayerBullet(stg, x - 36,  y - 2, 
                Fix(-5 * Fix::ONE), Fix(1 * Fix::ONE), BulletType::Spray3);
            break;
        }
        return 5;
    case 2: // beam
        PlayGunSound(SoundEffect::FireBeam);
        beamOffset = -beamOffset;
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 16, y - 3,
                Fix(16 * Fix::ONE), 0, BulletType::Beam1);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 16, y - 3 + 5 * -beamOffset,
                Fix(16 * Fix::ONE), 0, BulletType::Beam2);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 16, y - 3 + 5 * -beamOffset,
                Fix(16 * Fix::ONE), 0, BulletType::Beam3);
            break;
        }
        return 6;
    case 3: // cross
        PlayGunSound(SoundEffect::FireCross);
        switch (level)
        {
        case 0:
            if (crossDir)
            {
                SpawnPlayerBullet(stg, x - 10, y - 15,
                    Fix(5 * Fix::ONE), -Fix(5 * Fix::ONE),
                        BulletType::CrossNE12);
                SpawnPlayerBullet(stg, x - 27, y + 2,
                    -Fix(5 * Fix::ONE), Fix(5 * Fix::ONE),
                        BulletType::CrossSW12);
            }
            else
            {
                SpawnPlayerBullet(stg, x - 27, y - 15,
                    -Fix(5 * Fix::ONE), -Fix(5 * Fix::ONE),
                        BulletType::CrossNW12);
                SpawnPlayerBullet(stg, x - 10, y + 2,
                    Fix(5 * Fix::ONE), Fix(5 * Fix::ONE),
                        BulletType::CrossSE12);
            }
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 10, y - 15,
                Fix(5 * Fix::ONE), -Fix(5 * Fix::ONE), BulletType::CrossNE12);
            SpawnPlayerBullet(stg, x - 27, y + 2,
                -Fix(5 * Fix::ONE), Fix(5 * Fix::ONE), BulletType::CrossSW12);
            SpawnPlayerBullet(stg, x - 27, y - 15,
                -Fix(5 * Fix::ONE), -Fix(5 * Fix::ONE), BulletType::CrossNW12);
            SpawnPlayerBullet(stg, x - 10, y + 2,
                Fix(5 * Fix::ONE), Fix(5 * Fix::ONE), BulletType::CrossSE12);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 10, y - 15,
                Fix(5 * Fix::ONE), -Fix(5 * Fix::ONE), BulletType::CrossNE3);
            SpawnPlayerBullet(stg, x - 27, y + 2,
                -Fix(5 * Fix::ONE), Fix(5 * Fix::ONE), BulletType::CrossSW3);
            SpawnPlayerBullet(stg, x - 27, y - 15,
                -Fix(5 * Fix::ONE), -Fix(5 * Fix::ONE), BulletType::CrossNW3);
            SpawnPlayerBullet(stg, x - 10, y + 2,
                Fix(5 * Fix::ONE), Fix(5 * Fix::ONE), BulletType::CrossSE3);
            break;
        }
        crossDir = !crossDir;
        return 6;
    case 4: // flak
        PlayGunSound(SoundEffect::FireFlak);
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 15, y + 3,
                0, Fix(-9 * Fix::ONE), BulletType::FlakUp);
            SpawnPlayerBullet(stg, x - 15, y - 7, 
                0, Fix(9 * Fix::ONE), BulletType::FlakDown);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 15, y + 3,
                0, Fix(-9 * Fix::ONE), BulletType::FlakUp);
            SpawnPlayerBullet(stg, x - 15, y - 7, 
                0, Fix(9 * Fix::ONE), BulletType::FlakDown);
            SpawnPlayerBullet(stg, x - 4, y - 2, 
                Fix(9 * Fix::ONE), 0, BulletType::FlakForward);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 15, y + 3,
                0, Fix(-9 * Fix::ONE), BulletType::FlakUp3);
            SpawnPlayerBullet(stg, x - 15, y - 7, 
                0, Fix(9 * Fix::ONE), BulletType::FlakDown3);
            SpawnPlayerBullet(stg, x - 4, y - 2, 
                Fix(9 * Fix::ONE), 0, BulletType::FlakForward3);
            break;
        }
        return 20;
    case 5: // track
        PlayGunSound(SoundEffect::FireTrack);
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 4, y - 2, 
                Fix(6 * Fix::ONE), 0, BulletType::Track1);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 4, y - 10, 
                Fix(6 * Fix::ONE), 0, BulletType::Track2);
            SpawnPlayerBullet(stg, x - 4, y + 6, 
                Fix(6 * Fix::ONE), 0, BulletType::Track2);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 4, y - 10,
                Fix(6 * Fix::ONE), 0, BulletType::Track3);
            SpawnPlayerBullet(stg, x - 4, y + 6, 
                Fix(6 * Fix::ONE), 0, BulletType::Track3);
            break;
        }
        return 6;
    }
    return 0;
}
