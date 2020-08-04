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

PlayerSprite::PlayerSprite(int id, std::shared_ptr<Image> img, int x, int y,
    int flags, std::shared_ptr<Spritesheet> playerSprites,
    std::shared_ptr<Shooter> stg)
    : Sprite(id, img, x, y, flags), sheet(playerSprites), game(stg),
        rightEdge(S_WIDTH - img->width()), bottomEdge(S_HEIGHT - img->height()),
        deltaX(0), deltaY(0)
{
}

void PlayerSprite::tick()
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

    _x += deltaX;
    _y += deltaY;
    if (_x < 0) {
        _x = 0;
        deltaX = 0;
    } else if (_x > rightEdge) {
        _x = rightEdge;
        deltaX = 0;
    }
    if (_y < 32) {
        _y = 32;
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

void PlayerSprite::damage(int dmg)
{
    if (dmg > 0)
    {
        // kill player, or kill shield
    }
}

inline void PlayerSprite::setActiveSprite(int index)
{
    if (activeSprite != index)
    {
        updateImage(sheet->getImage(index));
        activeSprite = index;
    }
}
