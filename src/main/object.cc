/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// object.cc: base object implementation, including spawning

#include <memory>
#include "object.hh"

Sprite spawnObject(Shooter &stg, ObjectSpawn spawn, LayerScroll scroll)
{
    int id = stg.nextSpriteID();
    Sprite result = BlankSprite(id);
    int sx = spawn.xrel + S_WIDTH;
    int sy = spawn.y - stg.scroll.y;
    switch (static_cast<ObjectType>(spawn.type))
    {
    
    }
    if (spawn.spawnDelay)
        return DelaySpawnSprite(id, result, spawn.spawnDelay);
    else
        return result;
}
