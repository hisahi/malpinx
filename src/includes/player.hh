/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// player.hh: header for player.cc

#ifndef M_PLAYER_HH
#define M_PLAYER_HH

#include <memory>
#include "sprite.hh"
#include "m_game.hh"

class PlayerSprite : public Sprite
{
public:
    PlayerSprite(int id, std::shared_ptr<Image> img, int x, int y, int flags,
                std::shared_ptr<Spritesheet> playerSprites,
                std::shared_ptr<Shooter> stg);
    void tick();
    void damage(int dmg);
    void setActiveSprite(int index);
private:
    std::shared_ptr<Spritesheet> sheet;
    std::weak_ptr<Shooter> game;
    int activeSprite;
    int rightEdge;
    int bottomEdge;
    int deltaX;
    int deltaY;
    int resetTicks;
};

#endif // M_PLAYER_HH
