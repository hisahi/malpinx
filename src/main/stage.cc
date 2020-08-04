/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// stage.cc: stage implementation

#include "stage.hh"
#include "object.hh"

Stage::Stage(Shooter &g) : stg(g)
{

}

void Stage::spawnSprites(LayerScroll scroll)
{
    while (nextSpawn != objectSpawns.end())
    {
        ObjectSpawn &os = *nextSpawn;
        if (os.scrollX < scroll.x)
            break;
        spawnObject(stg, os, scroll);
        objectSpawns.pop_front();
        nextSpawn = objectSpawns.begin();
    }
}
