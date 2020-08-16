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

std::shared_ptr<Sprite> spawnObject(Shooter &stg, ObjectSpawn spawn,
                    LayerScroll scroll, int &layer)
{
    int id = stg.nextSpriteID();
    std::shared_ptr<Sprite> result;
    Fix sx = Fix(spawn.xrel + S_WIDTH);
    Fix sy = Fix(spawn.y);
    layer = 2;
    switch (static_cast<ObjectType>(spawn.type))
    {
    case ObjectType::Powerup:
        result = std::make_shared<PowerupSprite>(stg, id, sx, sy,
                            static_cast<PowerupType>(spawn.subtype));
        break;
    case ObjectType::Script:
        result = std::make_shared<ScriptSprite>(stg, id, spawn.subtype);
        break;
    case ObjectType::Enemy01:
        result = spawnEnemy<Enemy01>(stg, id, sx, sy, spawn);
        break;
    case ObjectType::Enemy02:
        result = spawnEnemy<Enemy02>(stg, id, sx, sy, spawn);
        break;
    case ObjectType::Enemy03:
        result = spawnEnemy<Enemy03>(stg, id, sx, sy, spawn);
        break;
    case ObjectType::Enemy04:
        result = spawnEnemy<Enemy04>(stg, id, sx, sy, spawn);
        break;
    case ObjectType::Enemy05:
        result = spawnEnemy<Enemy05>(stg, id, sx, sy, spawn);
        break;
    case ObjectType::Enemy06:
        result = spawnEnemy<Enemy06>(stg, id, sx, sy, spawn);
        break;
    case ObjectType::Boss1a:
        result = spawnEnemy<Boss1a>(stg, id, sx, sy, spawn);
        break;
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
