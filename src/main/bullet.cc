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

static int beamOffset = -1;

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
        case BulletType::Curve1:
        case BulletType::Curve2:
        case BulletType::Curve3:
            _img = stg.assets.bulletSprites->getImage(2);
            break;
        case BulletType::Beam1:
        case BulletType::Beam2:
        case BulletType::Beam3:
            _img = stg.assets.bulletSprites->getImage(5);
            break;
        case BulletType::FlakUp:
        case BulletType::FlakUp3:
            _img = stg.assets.bulletSprites->getImage(6);
            break;
        case BulletType::FlakDown:
        case BulletType::FlakDown3:
            _img = stg.assets.bulletSprites->getImage(7);
            break;
        case BulletType::FlakForward:
        case BulletType::FlakForward3:
            _img = stg.assets.bulletSprites->getImage(8);
            break;
        case BulletType::Track12:
        case BulletType::Track3:
            _img = stg.assets.bulletSprites->getImage(9);
            break;
    }
    if (_img)
    {
        _width = _img->width();
        _height = _img->height();
    }
}

void BulletSprite::tick()
{
    ++_ticks;
    
    if (_ticks == 3 &&
        (_type == BulletType::Curve1 
            || _type == BulletType::Curve2
            || _type == BulletType::Curve3))
    {
        _img = _stg.assets.bulletSprites->getImage(3);
        _fx -= Fix(3 * Fix::ONE);
        _fy -= Fix(10 * Fix::ONE);
    }
    if (_ticks == 8 && _type == BulletType::Curve3)
    {
        _img = _stg.assets.bulletSprites->getImage(4);
        _fx -= Fix(5 * Fix::ONE);
        _fy -= Fix(10 * Fix::ONE);
    }
    
    _fx += _dx;
    _fy += _dy;
    _x = static_cast<int>(_fx);
    _y = static_cast<int>(_fy);
    if (_x >= S_WIDTH || _y > _stg.stage->levelHeight || _y < -_height)
    {
        kill();
        return;
    }

    if (this->hasFlag(SPRITE_COLLIDE_FG))
        for (auto &layer : _stg.stage->terrainLayers)
            if (layer->hitsSprite(*_img, _stg.scroll, _x, _y))
            {
                stg->explode(_x + _img->width() / 2,
                            _y + _img->height() / 2,
                            _expl, true);
                kill();
                return;
            }

    if (_src == BulletSource::Player)
    {

    }
    // check player/enemy collision
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
    case 3: // curve
        PlayGunSound(SoundEffect::FireCurve);
        switch (level)
        {
        case 0:
            SpawnPlayerBullet(stg, x - 12, y - 10,
                Fix(10 * Fix::ONE), 0, BulletType::Curve1);
            break;
        case 1:
            SpawnPlayerBullet(stg, x - 12, y - 10,
                Fix(10 * Fix::ONE), 0, BulletType::Curve2);
            break;
        case 2:
            SpawnPlayerBullet(stg, x - 12, y - 10,
                Fix(10 * Fix::ONE), 0, BulletType::Curve3);
            break;
        }
        return 5;
    case 4: // flak
        PlayGunSound(SoundEffect::FireFlak);

        return 20;
    case 5: // track
        PlayGunSound(SoundEffect::FireTrack);

        return 6;
    }
    return 0;
}
