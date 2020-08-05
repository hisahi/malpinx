/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// bullet.hh: include for projectile objects

#ifndef M_BULLET_HH
#define M_BULLET_HH

#include "sprite.hh"
#include "m_game.hh"
#include "explode.hh"

enum class BulletType
{
    Pulse12, Pulse3,
    Spray12, Spray3,
    Beam1, Beam2, Beam3,
    Curve1, Curve2, Curve3,
    FlakUp, FlakDown, FlakForward, FlakUp3, FlakDown3, FlakForward3,
    Track12, Track3
};

enum class BulletSource
{
    None, Player, Enemy
};

class BulletSprite : public Sprite
{
public:
    BulletSprite(Shooter &stg, int id, int x, int y, Fix dx, Fix dy,
                    BulletType type, BulletSource source);
    void tick();
private:
    BulletType _type;
    BulletSource _src;
    int _height;
    Fix _fx;
    Fix _fy;
    Fix _dx;
    Fix _dy;
    ExplosionSize _expl;
    Shooter &_stg;
    int _damage;
};

int FireWeapon(Shooter &stg, int weapon, int level, int x, int y);

#endif // M_BULLET_HH
