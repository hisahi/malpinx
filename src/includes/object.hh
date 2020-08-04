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

Sprite spawnObject(Shooter &stg, ObjectSpawn spawn, LayerScroll scroll);

struct BlankSprite : public Sprite
{
    BlankSprite(int id) : Sprite(id, nullptr, 0, 0, SPRITE_NODRAW) { }
    void tick() { kill(); }
};

class DelaySpawnSprite : public Sprite
{
public:
    DelaySpawnSprite(int id, Sprite sprite, int delay)
        : Sprite(id, nullptr, S_WIDTH, S_HEIGHT,
            SPRITE_NODRAW | SPRITE_NOSCROLL | SPRITE_SURVIVE_OFF_SCREEN),
            _sprite(std::make_unique<Sprite>(sprite)), _delay(delay)
    {
    }

    std::unique_ptr<Sprite> unique()
    {
        std::unique_ptr<Sprite> uptr(this);
        _me = &uptr;
        return uptr;
    }

    void tick()
    {
        if (_delay == 0)
        {
            *_me = std::move(_sprite);
            kill();
            return;
        }
        --_delay;
    }
private:
    std::unique_ptr<Sprite> *_me;
    std::unique_ptr<Sprite> _sprite;
    int _delay;
};

#endif // M_OBJECT_HH
