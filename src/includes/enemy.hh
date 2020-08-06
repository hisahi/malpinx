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

class EnemySprite : public Sprite
{
public:
    EnemySprite(Shooter &stg, int id, int x, int y, int subtype);
    virtual void tick();
    // returns whether dead
    virtual bool damage(int dmg);
    virtual void explode();
    inline bool noDamage()
    {
        PlaySound(SoundEffect::EnemyNoDamage);
        return false;
    };
protected:
    Shooter &_stg;
};

#endif // M_ENEMY_HH
