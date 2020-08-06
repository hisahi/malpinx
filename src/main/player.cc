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

PlayerSprite::PlayerSprite(int id, std::shared_ptr<Image> img, int x, int y,
    int flags, std::shared_ptr<Spritesheet> playerSprites,
    std::shared_ptr<Shooter> stg)
    : Sprite(id, img, x, y, flags, SpriteType::Player), fireDelay(0),
        sheet(playerSprites), game(stg), deltaX(0), deltaY(0), invulTicks(0),
        rightEdge(S_WIDTH - img->width()),
        bottomEdge(S_GHEIGHT - img->height())
{
}

inline void PlayerSprite::moveTick()
{
    if ((-4 < deltaX && deltaX < 0) || (0 < deltaX && deltaX < 4))
        deltaX *= 2;
    if ((-4 < deltaY && deltaY < 0) || (0 < deltaY && deltaY < 4))
        deltaY *= 2;

    if (gameInputEdge.left)
        deltaX = -1;
    if (gameInputEdge.right)
        deltaX = 1;
    if (deltaX != 0 && !gameInput.left && !gameInput.right)
        deltaX = 0;

    if (gameInputEdge.up)
    {
        deltaY = -1;
        setActiveSprite(1);
    }
    if (gameInputEdge.down)
    {
        deltaY = 1;
        setActiveSprite(3);
    }
    if (deltaY != 0 && !gameInput.up && !gameInput.down)
    {
        setActiveSprite(deltaY > 0 ? 3 : 1);
        resetTicks = 4;
        deltaY = 0;
    }

    if (deltaY == -4)
        setActiveSprite(2);
    else if (deltaY == 4)
        setActiveSprite(4);

    _x += deltaX * (1 + game->speed) / 4;
    _y += deltaY * (1 + game->speed) / 4;
    if (_x < 0) {
        _x = 0;
        deltaX = 0;
    } else if (_x > rightEdge) {
        _x = rightEdge;
        deltaX = 0;
    }
    if (_y < 0) {
        _y = 0;
        deltaY = 0;
        setActiveSprite(1);
        resetTicks = 4;
    } else if (_y > bottomEdge) {
        _y = bottomEdge;
        deltaY = 0;
        setActiveSprite(3);
        resetTicks = 4;
    }
    
    if (activeSprite != 0 && deltaY == 0 && --resetTicks < 0)
        setActiveSprite(0);
}

void PlayerSprite::updateY(int y, int bottom)
{
    _y = y;
    bottomEdge = bottom - _img->height();
}

void PlayerSprite::onWeaponChange()
{
    fireDelay = S_TICKS / 8;
}

inline void PlayerSprite::fireTick()
{
    if (gameInput.fire && fireDelay == 0)
        fireDelay = FireWeapon(*stg, stg->activeWeapon,
                        stg->weaponLevels[stg->activeWeapon],
                        _x + 31, _y + 10);
    else if (fireDelay > 0)
        --fireDelay;
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
    PlaySound(SoundEffect::PlayerHit);
    if (dmg > 0)
    {
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
