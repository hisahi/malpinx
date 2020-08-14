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
    PlayerSprite(int id, std::shared_ptr<Image> img, Fix x, Fix y, int flags,
                std::shared_ptr<Spritesheet> playerSprites,
                std::shared_ptr<Shooter> stg);
    void moveTick();
    void fireTick();
    void collisionTick();
    void tick();
    void respawned();
    bool damage(int dmg);
    bool hasShield();
    void giveShield();
    void setActiveSprite(int index);
    void onWeaponChange();
    void updateY(Fix y, Fix bottom);
private:
    std::shared_ptr<Spritesheet> sheet;
    std::shared_ptr<Shooter> game;
    int fireDelay{0};
    int sigmaDelay{0};
    int activeSprite;
    Fix rightEdge;
    Fix bottomEdge;
    Fix deltaX;
    Fix deltaY;
    int resetTicks{0};
    int invulTicks{0};
    bool shield{false};
};

#endif // M_PLAYER_HH
