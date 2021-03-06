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
#include "songs.hh"
#include "fixrng.hh"

int ScaleFireTicks(Shooter &stg, int value);

class EnemySprite : public Sprite
{
public:
    // returns whether dead
    virtual void doEnemyTick() = 0;
    virtual bool damage(int dmg) override;
    virtual void explode();
    virtual void killEnemyByPlayer();
    virtual void killEnemy();
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
    inline bool hitsPlayer() const
    {
        return hits(_stg.player.get());
    };
    inline bool hitsAnyForeground() const
    {
        if (_stg.stage)
            for (auto &layer : _stg.stage->terrainLayers)
                if (layer && hitsForeground(*layer, _stg.scroll))
                    return true;
        return false;
    };
    inline void damagePlayerOnTouch() const
    {
        if (hitsPlayer())
            _stg.player->damage(10);
    };
    inline void move()
    {
        _x += _dx;
        _y += _dy;
    };
    virtual Fix2D trackTarget() const override
    {
        return Fix2D(_width / 2_x, _height / 2_x);
    };
protected:
    EnemySprite(Shooter &stg, int id, Fix x, Fix y, int hp, PowerupType drop);
    Shooter &_stg;
    Fix _dx{0}, _dy{0};
    int _score{0};
    int _flash{0};
    int _hp{0};
    bool _redShift{false};
    bool _invulnerable{false};
    ExplosionSize _esize{ExplosionSize::Medium1};
    PowerupType _drop;
};

class BossSprite : public EnemySprite
{
protected:
    BossSprite(Shooter &stg, int id, Fix x, Fix y, int hp, PowerupType drop)
        : EnemySprite(stg, id, x, y, hp, drop) { }
    virtual void doBossTick() = 0;
    virtual void initBoss() { }
    bool _bossInit{false};
    bool _scrollStopped{false};
    Fix _oldXScroll{0_x};
    Fix _targetXScroll{Fix(S_WIDTH)};

    void isLevelBoss()
    {
        StopSong();
        PlaySong(MusicTrack::Boss);
    }

public:
    void doEnemyTick() override
    {
        if (!_bossInit)
        {
            _bossInit = true;
            initBoss();
        }
        doBossTick();
    };
};

class Enemy01 : public EnemySprite
{
    int sineMul;
public:
    Enemy01(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy02 : public EnemySprite
{
    bool yflip;
public:
    Enemy02(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy03 : public EnemySprite
{
    int fireTicks{-1};
    Fix finalY;
public:
    Enemy03(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy04 : public EnemySprite
{
    int fireTicks{-1};
public:
    Enemy04(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy05 : public EnemySprite
{
    int moveTicks{-1};
    Fix speed{0};
    bool fg{false};
public:
    Enemy05(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy06 : public EnemySprite
{
    int moveTicks{45};
    int fireTicks{-1};
    int waitTicks{-1};
public:
    Enemy06(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy07 : public EnemySprite
{
public:
    Enemy07(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
    void explode() override;
};

class Enemy08 : public EnemySprite
{
    bool yflip;
    Fix trackAngle;
    Fix pointAngle;
    Fix turnSpeed;
    int fireTicks{-1};
public:
    Enemy08(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy09 : public EnemySprite
{
    int fireTicks{-1};
public:
    Enemy09(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy10 : public EnemySprite
{
    int fireTicks{-1};
    int sineMul;
public:
    Enemy10(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy11 : public EnemySprite
{
    int fireTicks{-1};
    int bombTicks{-1};
public:
    Enemy11(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy12 : public EnemySprite
{
    int fireTicks{-1};
    int angle{0};
    int angleDiff{1};
public:
    Enemy12(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy13 : public EnemySprite
{
    Fix origY;
    Fix ddy;
public:
    Enemy13(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Enemy14 : public EnemySprite
{
    Fix origY;
    Fix ddy;
    int fireTicks{-1};
public:
    Enemy14(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doEnemyTick() override;
};

class Boss1a : public BossSprite
{
    Fix minY{0_x}, maxY{Fix(S_HEIGHT - 64)};
    Fix2D vecPl;
    int mode{0};
    int modeTicks{0};
public:
    Boss1a(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doBossTick() override;
    virtual void killEnemy() override;
    virtual void explode() override;
};

class Boss1b : public BossSprite
{
    Fix minY{0_x}, maxY{Fix(S_HEIGHT - 64)};
    Fix2D vecPl;
    FixRandom explosionRng{647875107};
    int mode{0};
    int modeTicks{0};
    int spawnTicks{0};
    int explodeTicks{0};
public:
    Boss1b(Shooter &stg, int id, Fix x, Fix y, int subtype, PowerupType drop);
    virtual void doBossTick() override;
    virtual void killEnemy() override;
    virtual void explode() override;
};

#endif // M_ENEMY_HH
