/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// powerup.cc: powerup object

#include "stage.hh"
#include "powerup.hh"
#include "sfx.hh"
#include "player.hh"

PowerupSprite::PowerupSprite(Shooter &stg,
                    int id, Fix x, Fix y, PowerupType type)
    : Sprite(id, nullptr, x, y, SPRITE_COLLIDE_SPRITES,
        SpriteType::Pickup), _stg(stg), _type(type)
{
    switch (type)
    {
    case PowerupType::Score:
        updateImage(stg.assets.powerupSprites->getImage(0));
        break;
    case PowerupType::OneUp:
        updateImage(stg.assets.powerupSprites->getImage(1));
        break;
    case PowerupType::Pulse:
        updateImage(stg.assets.powerupSprites->getImage(2));
        break;
    case PowerupType::Spray:
        updateImage(stg.assets.powerupSprites->getImage(3));
        break;
    case PowerupType::Beam:
        updateImage(stg.assets.powerupSprites->getImage(4));
        break;
    case PowerupType::Track:
        updateImage(stg.assets.powerupSprites->getImage(5));
        break;
    case PowerupType::Drone:
        updateImage(stg.assets.powerupSprites->getImage(6));
        break;
    case PowerupType::Special:
        updateImage(stg.assets.powerupSprites->getImage(7));
        break;
    default:
        kill();
    }
}

void PowerupSprite::tick()
{
    if (hits(_stg.player.get()))
        collect();
}

void PowerupSprite::turnToScore(int score)
{
    PlaySound(SoundEffect::ScorePowerup);
    _stg.spawnScore(_x + 8, _y, 2000);
}

void PowerupSprite::collect()
{
    switch (_type)
    {
    case PowerupType::Score:
        break;
    case PowerupType::OneUp:
        if (!_stg.collectOneUp())
            turnToScore(5000);
        break;
    case PowerupType::Pulse:
        if (!_stg.collectWeapon(0))
            turnToScore(5000);
        break;
    case PowerupType::Spray:
        if (!_stg.collectWeapon(1))
            turnToScore(5000);
        break;
    case PowerupType::Beam:
        if (!_stg.collectWeapon(2))
            turnToScore(5000);
        break;
    case PowerupType::Track:
        if (!_stg.collectWeapon(3))
            turnToScore(5000);
        break;
    case PowerupType::Drone:
        break;
    case PowerupType::Special:
        break;
    }
    kill();
}
