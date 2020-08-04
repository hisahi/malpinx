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

enum class BulletType
{
    Pulse1, Pulse2, Pulse3,
    Spray1, Spray2, Spray3,
    Curve1, Curve2, Curve3,
    Beam1, Beam2, Beam3,
    Flak1, Flak2, Flak3,
    Track1, Track2, Track3
};

class BulletSprite : public Sprite
{
public:
    BulletSprite(int id, int x, int y, BulletType type);
    void tick();
private:
    BulletType _type;
    int _height;
    Fix _fx;
    Fix _fy;
    Fix _dx;
    Fix _dy;
};

#endif // M_BULLET_HH
