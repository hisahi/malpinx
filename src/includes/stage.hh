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
#include <deque>
#include "stage.hh"
#include "layer.hh"
#include "m_game.hh"
#include "powerup.hh"

constexpr int STAGE_COUNT = 6;

struct ObjectSpawn
{
    int scrollX;
    int type;
    int spawnDelay;
    int flags;
    int subtype;
    int y;
    int xrel;
    PowerupType drop;
};

struct Stage
{
    Stage(Shooter &g);
    Shooter &stg;
    std::vector<std::unique_ptr<BackgroundLayer>> backgroundLayers;
    std::vector<std::unique_ptr<ForegroundLayer>> terrainLayers;
    std::vector<std::unique_ptr<BackgroundLayer>> foregroundLayers;
    std::deque<ObjectSpawn> objectSpawns;
    std::deque<ObjectSpawn> delayedObjectSpawns;
    std::deque<ObjectSpawn>::iterator nextSpawn;
    int levelHeight{S_GHEIGHT};
    int spawnLevelY{100};

    void spawnSprites(LayerScroll scroll);
};

#endif // M_STAGE_HH
