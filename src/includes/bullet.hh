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
    Pulse1, Pulse2, Pulse3,
    Spray1, Spray2, Spray3,
    Beam1, Beam2, Beam3,
    Track1, Track2, Track3,
    Enemy3, Enemy4, Enemy6,
    Sigma,
    SuicideBullet
};

enum class BulletSource
{
    Player, Enemy
};

class BulletSprite : public Sprite
{
public:
    BulletSprite(Shooter &stg, int id, Fix x, Fix y, Fix dx, Fix dy,
                    BulletType type, BulletSource source);
    void tick() override;
    void explode();
private:
    BulletType _type;
    BulletSource _src;
    Fix2D _pos;
    Fix2D _vel;
    ExplosionSize _expl;
    Shooter &_stg;
    int _damage;
    short _frame{1};
    short _minFrame{1};
    short _maxFrame{1};
    short _animSpeed{1};
    bool _pierce{false};
    bool _sigma{false};
    std::vector<std::reference_wrapper<Sprite>> hitTargets;
};

void ResetBullets(int stageNum);
int FireWeapon(Shooter &stg, int weapon, int level, Fix x, Fix y);
int FireSigma(Shooter &stg, Fix x, Fix y);
void FireEnemyBullet(Shooter &stg, BulletType type, Fix x, Fix y,
            Fix dx, Fix dy, int scaleMode = 1);
void FireSuicideBullet(Shooter &stg, Fix x, Fix y);
inline void FireEnemyBullet(Shooter &stg, BulletType type, Fix x, Fix y,
            Fix2D d, int scaleMode = 1)
{
    FireEnemyBullet(stg, type, x, y, d.x, d.y, scaleMode);
}

#endif // M_BULLET_HH
