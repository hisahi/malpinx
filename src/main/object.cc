/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// object.cc: base object implementation, including spawning

#include <memory>
#include "object.hh"
#include "fonts.hh"
#include "powerup.hh"
#include "enemy.hh"
#include "sprite.hh"

template <typename T>
inline std::shared_ptr<Sprite> spawnEnemy(Shooter &stg, int id,
                    Fix x, Fix y, ObjectSpawn os)
{
    return std::make_shared<T>(stg, id, x, y, os.subtype, os.drop);
}

#define CASE_ENEMY_SPAWN(T) \
    case ObjectType:: T :\
        result = spawnEnemy<T>(stg, id, sx, sy, spawn); \
        break;

std::shared_ptr<Sprite> spawnObject(Shooter &stg, ObjectSpawn spawn,
                    LayerScroll scroll, int &layer)
{
    int id = stg.nextSpriteID();
    layer = 2;
    if (spawn.minDifficulty > static_cast<int>(stg.difficulty))
        return std::make_shared<BlankSprite>(id);
    std::shared_ptr<Sprite> result;
    Fix sx = Fix(spawn.xrel + S_WIDTH);
    Fix sy = Fix(spawn.y);
    switch (static_cast<ObjectType>(spawn.type))
    {
    case ObjectType::Powerup:
        result = std::make_shared<PowerupSprite>(stg, id, sx, sy,
                            static_cast<PowerupType>(spawn.subtype));
        break;
    case ObjectType::Script:
        result = std::make_shared<ScriptSprite>(stg, id, spawn.subtype);
        break;
    CASE_ENEMY_SPAWN(Enemy01);
    CASE_ENEMY_SPAWN(Enemy02);
    CASE_ENEMY_SPAWN(Enemy03);
    CASE_ENEMY_SPAWN(Enemy04);
    CASE_ENEMY_SPAWN(Enemy05);
    CASE_ENEMY_SPAWN(Enemy06);
    CASE_ENEMY_SPAWN(Enemy07);
    CASE_ENEMY_SPAWN(Enemy08);
    CASE_ENEMY_SPAWN(Enemy09);
    CASE_ENEMY_SPAWN(Enemy10);
    CASE_ENEMY_SPAWN(Enemy11);
    CASE_ENEMY_SPAWN(Enemy12);
    CASE_ENEMY_SPAWN(Enemy13);
    CASE_ENEMY_SPAWN(Enemy14);
    CASE_ENEMY_SPAWN(Boss1a);
    CASE_ENEMY_SPAWN(Boss1b);
    default:
        result = std::make_shared<BlankSprite>(id);
    }
    return result;
}

std::vector<std::shared_ptr<Sprite>> &getLayer(Shooter &stg, int layer)
{
    switch (layer)
    {
    case 0:
        return stg.spriteLayer0;
    case 1:
        return stg.spriteLayer1;
    case 2:
        return stg.spriteLayer2;
    case 3:
        return stg.spriteLayer3;
    case 4:
        return stg.spriteLayer4;
    default:
        throw std::runtime_error("invalid layer");
    }
}

void addObject(Shooter &stg, int layer, std::shared_ptr<Sprite> holder)
{
    getLayer(stg, layer).push_back(std::move(holder));
}

void spawnAndAddObject(Shooter &stg, ObjectSpawn spawn, LayerScroll scroll)
{
    int layer;
    addObject(stg, layer, spawnObject(stg, spawn, scroll, layer));
}

ScoreSprite::ScoreSprite(int id, Fix x, Fix y, int score)
    : Sprite(id, nullptr, x, y, SPRITE_DEFAULT, SpriteType::Temporary),
        _ticks(S_TICKS * 3 / 2)
{
    std::string scoreStr = std::to_string(score);
    std::shared_ptr<Image> tmp = std::make_shared<Image>
                                    (scoreStr.length() * 4, 8);
    int cx = 0;
    for (const char &c : scoreStr)
    {
        thinFont.blitFast(*tmp, c - '0', cx, 0);
        cx += 4;
    }
    updateImage(tmp);
    _x -= _width / 2;
}
