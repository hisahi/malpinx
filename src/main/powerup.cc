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
                    int id, int x, int y, PowerupType type)
    : Sprite(id, nullptr, x, y, SPRITE_COLLIDE_SPRITES,
        SpriteType::Pickup), _type(type), _stg(stg)
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
    case PowerupType::Curve:
        updateImage(stg.assets.powerupSprites->getImage(4));
        break;
    case PowerupType::Beam:
        updateImage(stg.assets.powerupSprites->getImage(5));
        break;
    case PowerupType::Flak:
        updateImage(stg.assets.powerupSprites->getImage(6));
        break;
    case PowerupType::Track:
        updateImage(stg.assets.powerupSprites->getImage(7));
        break;
    }
}

void PowerupSprite::tick()
{
    if (hits(*_stg.getPlayer()))
        collect();
}

void PowerupSprite::collect()
{
    switch (_type)
    {
    case PowerupType::Score:
        break;
    case PowerupType::OneUp:
        _stg.collectOneUp();
        break;
    case PowerupType::Pulse:
        _stg.collectWeapon(0);
        break;
    case PowerupType::Spray:
        _stg.collectWeapon(1);
        break;
    case PowerupType::Curve:
        _stg.collectWeapon(2);
        break;
    case PowerupType::Beam:
        _stg.collectWeapon(3);
        break;
    case PowerupType::Flak:
        _stg.collectWeapon(4);
        break;
    case PowerupType::Track:
        _stg.collectWeapon(5);
        break;
    }
    kill();
}
