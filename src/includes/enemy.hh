/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// enemy.hh: include for enemy objects

#ifndef M_ENEMY_HH
#define M_ENEMY_HH

#include "object.hh"
#include "sprite.hh"
#include "sfx.hh"
#include "player.hh"
#include "explode.hh"

int ScaleFireTicks(Shooter &stg, int value);

class EnemySprite : public Sprite
{
public:
    // returns whether dead
    virtual void doEnemyTick() = 0;
    virtual bool damage(int dmg) override;
    virtual void explode();
    void tick() override
    {
        if (_flash) _flash -= 2;
        doEnemyTick();
        ++_ticks;
    }
    virtual void blit(Image &fb, int xoff, int yoff) const;
    inline void flashDamage()
    {
        _flash = 10;
    };
    inline void gotDamage()
    {
        PlaySound(SoundEffect::EnemyDamage);
        flashDamage();
    };
    inline bool gotNoDamage()
    {
        PlaySound(SoundEffect::EnemyNoDamage);
        return false;
    };
    inline bool hitsPlayer()
    {
        return hits(_stg.player.get());
    };
    inline void damagePlayerOnTouch()
    {
        if (hitsPlayer())
            _stg.player->damage(10);
    };
    inline void move()
    {
        _x += _dx;
        _y += _dy;
    };
protected:
    EnemySprite(Shooter &stg, int id, Fix x, Fix y, int hp);
    Shooter &_stg;
    Fix _dx{0}, _dy{0};
    int _score{0};
    int _flash{0};
    int _hp{0};
    ExplosionSize _esize{ExplosionSize::Medium1};
};

class Enemy01 : public EnemySprite
{
    int sineMul;
public:
    Enemy01(Shooter &stg, int id, Fix x, Fix y, int subtype);
    virtual void doEnemyTick();
};

class Enemy02 : public EnemySprite
{
    bool yflip;
public:
    Enemy02(Shooter &stg, int id, Fix x, Fix y, int subtype);
    virtual void doEnemyTick();
};

class Enemy03 : public EnemySprite
{
    int fireTicks{-1};
    Fix finalY;
public:
    Enemy03(Shooter &stg, int id, Fix x, Fix y, int subtype);
    virtual void doEnemyTick();
};

class Enemy04 : public EnemySprite
{
    int fireTicks{-1};
public:
    Enemy04(Shooter &stg, int id, Fix x, Fix y, int subtype);
    virtual void doEnemyTick();
};

#endif // M_ENEMY_HH
