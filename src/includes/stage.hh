/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// stage.hh: header file for stage.cc

#ifndef M_STAGE_HH
#define M_STAGE_HH

#include <cstdint>
#include <forward_list>
#include "stage.hh"
#include "layer.hh"
#include "m_game.hh"

struct ObjectSpawn
{
    int scrollX;
    char type;
    int spawnDelay;
    int flags;
    int subtype;
    int y;
    int xrel;
};

struct Stage
{
    Stage(Shooter &g);
    Shooter &stg;
    std::vector<std::unique_ptr<BackgroundLayer>> backgroundLayers;
    std::vector<std::unique_ptr<ForegroundLayer>> terrainLayers;
    std::vector<std::unique_ptr<BackgroundLayer>> foregroundLayers;
    std::forward_list<ObjectSpawn> objectSpawns;
    std::forward_list<ObjectSpawn>::iterator nextSpawn;

    void spawnSprites(LayerScroll scroll);
};

#endif // M_STAGE_HH
