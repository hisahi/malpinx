/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// object.hh: header file for object.cc

#ifndef M_OBJECT_HH
#define M_OBJECT_HH

#include <cstdint>
#include <forward_list>
#include "stage.hh"
#include "layer.hh"
#include "m_game.hh"
#include "fix.hh"

enum class ObjectType
{
    Blank,
    Script,
    Powerup,

    Enemy01 = 101,
    Enemy02, Enemy03, Enemy04, Enemy05, Enemy06, Enemy07, Enemy08,
    Enemy09, Enemy10, Enemy11, Enemy12, Enemy13, Enemy14,

    Boss1a = 201, Boss1b
};

std::shared_ptr<Sprite> spawnObject(Shooter &stg, ObjectSpawn spawn,
                    LayerScroll scroll, int &layer);
void addObject(Shooter &stg, int layer, std::shared_ptr<Sprite> holder);
void spawnAndAddObject(Shooter &stg, ObjectSpawn spawn, LayerScroll scroll);

struct BlankSprite : public Sprite
{
    BlankSprite(int id) : Sprite(id, nullptr, 0_x, 0_x, SPRITE_NODRAW,
            SpriteType::Other) { }
    void tick() { kill(); }
};

class ScoreSprite : public Sprite
{
public:
    ScoreSprite(int id, Fix x, Fix y, int score);
    void tick()
    {
        if (!--_ticks)
            kill();
    }
private:
    int _ticks;
};

class ScriptSprite : public Sprite
{
public:
    ScriptSprite(Shooter &stg, int id, int scriptNo);
    void tick();
private:
    Shooter &_stg;
    int _script;
};

#endif // M_OBJECT_HH
