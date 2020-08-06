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

enum class ObjectType
{
    Blank,
    Script,
    Powerup
};

std::unique_ptr<Sprite> spawnObject(Shooter &stg, ObjectSpawn spawn,
                    LayerScroll scroll, int &layer);
void addObject(Shooter &stg, int layer, std::unique_ptr<Sprite> holder);
void spawnAndAddObject(Shooter &stg, ObjectSpawn spawn, LayerScroll scroll);

struct BlankSprite : public Sprite
{
    BlankSprite(int id) : Sprite(id, nullptr, 0, 0, SPRITE_NODRAW,
            SpriteType::Other) { }
    void tick() { kill(); }
};

class DelaySpawnSprite : public Sprite
{
public:
    DelaySpawnSprite(Shooter &stg, int id, std::unique_ptr<Sprite> &&sprite,
                    int delay, int layer);
    std::unique_ptr<Sprite> &&replace();
    void tick();
private:
    Shooter &_stg;
    std::unique_ptr<Sprite> _sprite;
    int _delay;
    int _layer;
};

class ScoreSprite : public Sprite
{
public:
    ScoreSprite(int id, int x, int y, int score);
    void tick()
    {
        if (!--_ticks)
            kill();
    }
private:
    int _ticks;
};

#endif // M_OBJECT_HH
