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
    void tick() override;
    void respawned();
    bool damage(int dmg) override;
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

class DroneSprite : public Sprite
{
public:
    DroneSprite(Shooter &stg, int id, PlayerSprite &player, int droneNum);
    void blit(Image &fb, int xoff, int yoff) const override;
    void tick() override;
    void explode();
private:
    Shooter &game;
    PlayerSprite &player;
    int droneNum;
    int spawnTicks{30};
    int damageTicks{0};
    Fix centerX;
    Fix centerY;
    std::vector<Sprite *> droneHitTargets;
};

#endif // M_PLAYER_HH
