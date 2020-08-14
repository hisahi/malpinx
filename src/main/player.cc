/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// player.cc: code for player ship

#include <iostream>
#include <memory>
#include "image.hh"
#include "m_game.hh"
#include "player.hh"
#include "input.hh"
#include "sfx.hh"
#include "bullet.hh"
#include "fix.hh"

PlayerSprite::PlayerSprite(int id, std::shared_ptr<Image> img, Fix x, Fix y,
    int flags, std::shared_ptr<Spritesheet> playerSprites,
    std::shared_ptr<Shooter> stg)
    : Sprite(id, img, x, y, flags, SpriteType::Player), fireDelay(0),
        sheet(playerSprites), game(stg), deltaX(0), deltaY(0), invulTicks(0),
        rightEdge(Fix(S_WIDTH - img->width())),
        bottomEdge(Fix(S_GHEIGHT - img->height()))
{
}

inline void PlayerSprite::moveTick()
{
    if ((-4 < deltaX && deltaX < 0) || (0 < deltaX && deltaX < 4))
        deltaX *= 2;
    if ((-4 < deltaY && deltaY < 0) || (0 < deltaY && deltaY < 4))
        deltaY *= 2;

    if (gameInputEdge.left)
        deltaX = -1_x;
    if (gameInputEdge.right)
        deltaX = 1_x;
    if (deltaX != 0 && !gameInput.left && !gameInput.right)
        deltaX = 0_x;

    if (gameInputEdge.up)
    {
        deltaY = -1_x;
        setActiveSprite(1);
    }
    if (gameInputEdge.down)
    {
        deltaY = 1_x;
        setActiveSprite(3);
    }
    if (deltaY != 0 && !gameInput.up && !gameInput.down)
    {
        setActiveSprite(deltaY > 0 ? 3 : 1);
        resetTicks = 4;
        deltaY = 0_x;
    }

    if (deltaY == -4)
        setActiveSprite(2);
    else if (deltaY == 4)
        setActiveSprite(4);
    
    _x += deltaX * (1 + game->playerSpeed) / 4;
    _y += deltaY * (1 + game->playerSpeed) / 4;
    if (_x < 0) {
        _x = 0_x;
        deltaX = 0_x;
    } else if (_x > rightEdge) {
        _x = rightEdge;
        deltaX = 0_x;
    }
    if (_y < 0) {
        _y = 0_x;
        deltaY = 0_x;
        setActiveSprite(1);
        resetTicks = 4;
    } else if (_y > bottomEdge) {
        _y = bottomEdge;
        deltaY = 0_x;
        setActiveSprite(3);
        resetTicks = 4;
    }
    
    if (activeSprite != 0 && deltaY == 0 && --resetTicks < 0)
        setActiveSprite(0);
}

void PlayerSprite::updateY(Fix y, Fix bottom)
{
    _y = y;
    bottomEdge = bottom - _img->height();
}

void PlayerSprite::onWeaponChange()
{
}

inline void PlayerSprite::fireTick()
{
    if (gameInput.fire && fireDelay == 0)
        fireDelay = FireWeapon(*game, game->activeWeapon,
                game->weaponLevels[game->activeWeapon], _x + 31, _y + 10);
    else if (fireDelay > 0)
        --fireDelay;
    if (gameInputEdge.sigma && sigmaDelay == 0 && game->useSigma())
        sigmaDelay = FireSigma(*game, _x + 31, _y + 10);
    else if (sigmaDelay > 0)
        --sigmaDelay;
}

inline void PlayerSprite::collisionTick()
{
    if (invulTicks)
    {
        _flags ^= SPRITE_NODRAW;
        if (!--invulTicks)
            _flags &= ~SPRITE_NODRAW;
        return;
    }

    for (auto &layer : stg->stage->terrainLayers)
    {
        if (layer->hitsSprite(*_img, stg->scroll, _x, _y))
        {
            stg->killPlayer();
            return;
        }
    }
}

bool PlayerSprite::hasShield()
{
    return shield;
}

void PlayerSprite::giveShield()
{
    PlaySound(SoundEffect::GotShield);
    shield = true;
}

void PlayerSprite::tick()
{
    moveTick();
    fireTick();
    collisionTick();
}

void PlayerSprite::respawned()
{
    invulTicks = S_TICKS * 3;
}

bool PlayerSprite::damage(int dmg)
{
    if (invulTicks) return false;
    PlaySound(SoundEffect::PlayerHit);
    if (dmg > 0)
    {
        if (shield)
        {
            PlaySound(SoundEffect::LostShield);
            shield = false;
            invulTicks = S_TICKS / 2;
            return false;
        }
        game->killPlayer();
        return true;
    }
    return false;
}

inline void PlayerSprite::setActiveSprite(int index)
{
    if (activeSprite != index)
    {
        updateImage(sheet->getImage(index));
        activeSprite = index;
    }
}
