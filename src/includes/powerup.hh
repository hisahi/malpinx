/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// powerup.hh: include for powerup objects

#ifndef M_POWERUP_HH
#define M_POWERUP_HH

#include "object.hh"
#include "sprite.hh"

enum class PowerupType
{
    Score, OneUp, Pulse, Spray, Beam, Track, Drone, Special
};

class PowerupSprite : public Sprite
{
public:
    PowerupSprite(Shooter &stg, int id, Fix x, Fix y, PowerupType type);
    void tick() override;
    bool damage(int dmg) override { return false; }
    void collect();
    void turnToScore(int score);
private:
    Shooter &_stg;
    PowerupType _type;
};

#endif // M_POWERUP_HH
