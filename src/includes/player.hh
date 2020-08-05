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
    void moveTick();
    void fireTick();
    void collisionTick();
    void tick();
    void respawned();
    void damage(int dmg);
    void setActiveSprite(int index);
    void onWeaponChange();
    void updateY(int y, int bottom);
private:
    std::shared_ptr<Spritesheet> sheet;
    std::shared_ptr<Shooter> game;
    int fireDelay;
    int activeSprite;
    int rightEdge;
    int bottomEdge;
    int deltaX;
    int deltaY;
    int resetTicks;
    int invulTicks;
};

#endif // M_PLAYER_HH
