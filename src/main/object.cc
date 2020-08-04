/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// object.cc: base object implementation, including spawning

#include "object.hh"

Sprite spawnObject(Shooter &stg, ObjectSpawn spawn, LayerScroll scroll)
{
    int id = stg.nextSpriteID();
    switch (spawn.type)
    {
    default:
        return BlankSprite(id);
    }
}
