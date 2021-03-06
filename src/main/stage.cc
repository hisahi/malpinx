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
    for (auto it = delayedObjectSpawns.begin();
            it != delayedObjectSpawns.end();)
    {
        if (--it->spawnDelay <= 0)
        {
            spawnAndAddObject(stg, *it, scroll);
            it = delayedObjectSpawns.erase(it);
        } else
            ++it;
    }
    Fix fsx = scroll.x + S_WIDTH;
    while (nextSpawn != objectSpawns.end())
    {
        ObjectSpawn &os = *nextSpawn;
        if (os.scrollX > fsx)
            break;
        if (os.spawnDelay)
            delayedObjectSpawns.push_back(os);
        else
            spawnAndAddObject(stg, os, scroll);
        objectSpawns.pop_front();
        nextSpawn = objectSpawns.begin();
    }
}

void Stage::skipObjects(LayerScroll scroll)
{
    Fix fsx = scroll.x + S_WIDTH;
    while (nextSpawn != objectSpawns.end())
    {
        ObjectSpawn &os = *nextSpawn;
        if (os.scrollX > fsx)
            break;
        objectSpawns.pop_front();
        nextSpawn = objectSpawns.begin();
    }
}

void Stage::hideLayer(int index)
{
    backgroundLayers[index]->hide();
}

void Stage::showLayer(int index)
{
    backgroundLayers[index]->show();
}
