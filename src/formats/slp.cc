/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// tip.cc: code for loading *.tip and *.tsp files

#include <vector>
#include <memory>
#include <sstream>
#include "formats.hh"
#include "gamedata.hh"
#include "stage.hh"
#include "tiled.hh"
#include "powerup.hh"

inline std::string SLP_build_message(const std::string& message,
    const std::string& path)
{
    std::ostringstream stream;
    stream << message << ": " << path << ".slp";
    return stream.str();
}

Stage LoadStage(const std::string &path, Shooter &stg)
{
    auto stream = OpenDataFile(path + ".slp");
    bool shared = false;
    if (stream.fail())
        throw std::runtime_error(SLP_build_message(
                        "Cannot open stage file", path));
    stream.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
    if (ReadUInt16(stream) != 4)
        throw std::runtime_error(SLP_build_message(
                        "Cannot open stage file", path));

    Stage stage(stg);
    int levelHeight = ReadUInt16(stream);
    if (levelHeight < 208)
        throw std::runtime_error(SLP_build_message(
                        "Unexpected stage height", path));
    int spawnLevelY = ReadUInt16(stream);
    int layers = ReadUInt16(stream);
    std::array<char, 14> filenameArr;
    std::string filename;
    for (int i = 0; i < layers; ++i)
    {
        int layerType = ReadUInt8(stream);
        int layerMeta = ReadUInt8(stream);
        stream.read(&filenameArr[0], 14);
        filename = std::string(filenameArr.data());

        Fix layerXMult = ReadFix(stream);
        Fix layerYMult = ReadFix(stream);
        int offsetX = ReadInt32(stream);
        int offsetY = ReadInt32(stream);
        std::shared_ptr<Tilemap> tilemap;
        std::string tilemapSheetName;
        ReadUInt32(stream);
        ReadUInt32(stream);

        switch (layerType >> 4)
        {
        case 1:
            tilemap = std::make_shared<Tilemap>(
                        LoadTilemap(filename, tilemapSheetName));
            break;
        }

        switch (layerType)
        {
        case 0x00:
            stage.backgroundLayers.push_back(
                std::make_unique<BackgroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x10:
            stage.backgroundLayers.push_back(
                std::make_unique<BackgroundTileLayer>(
                    std::make_shared<Spritesheet>(
                        LoadSpritesheet(tilemapSheetName)),
                    tilemap,
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x20:
            stage.backgroundLayers.push_back(
                std::make_unique<NonTiledBackgroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x30:
            stage.backgroundLayers.push_back(
                std::make_unique<HTiledBackgroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x40:
            stage.backgroundLayers.push_back(
                std::make_unique<HTiledAdditiveBackgroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x50:
            stage.backgroundLayers.push_back(
                std::make_unique<HTiledParallaxBackgroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult, layerMeta));
            break;
        case 0x60:
            stage.backgroundLayers.push_back(
                std::make_unique<HTiledWavyBackgroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult, layerMeta));
            break;

        case 0x01:
            stage.terrainLayers.push_back(
                std::make_unique<ForegroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x11:
            stage.terrainLayers.push_back(
                std::make_unique<ForegroundTileLayer>(
                    std::make_shared<Spritesheet>(
                        LoadSpritesheet(tilemapSheetName)),
                    tilemap,
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x21:
            stage.terrainLayers.push_back(
                std::make_unique<NonTiledForegroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult));
            break;

        case 0x02:
            stage.foregroundLayers.push_back(
                std::make_unique<BackgroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x12:
            stage.foregroundLayers.push_back(
                std::make_unique<BackgroundTileLayer>(
                    std::make_shared<Spritesheet>(
                        LoadSpritesheet(tilemapSheetName)),
                    tilemap,
                    offsetX, offsetY, layerXMult, layerYMult));
            break;
        case 0x22:
            stage.foregroundLayers.push_back(
                std::make_unique<NonTiledBackgroundLayer>(
                    std::make_shared<Image>(LoadPIC(filename)),
                    offsetX, offsetY, layerXMult, layerYMult));
            break;

        default:
            continue;
        }
    }

    stage.levelHeight = levelHeight;
    stage.spawnLevelY = spawnLevelY;

    std::int32_t spriteScrollX = 0, deltaX;
    int spriteType, spriteDelay, spriteFlags, spriteSubtype,
        spriteY, spriteX, spriteDrop, spriteMinDiff;
    for (;;)
    {
        ReadInt32(stream, deltaX);
        if (deltaX < 0) break;

        spriteScrollX += deltaX;

        for (;;)
        {
            spriteType = ReadUInt8(stream);
            if (!spriteType) break;
            spriteDelay = ReadUInt8(stream);
            spriteDelay |= ReadUInt16(stream) << 8;
            spriteFlags = ReadUInt16(stream);
            spriteSubtype = ReadUInt16(stream);
            spriteY = ReadInt32(stream);
            spriteX = ReadInt32(stream);
            spriteDrop = ReadUInt8(stream);
            spriteMinDiff = ReadUInt8(stream);
            ReadUInt8(stream);
            ReadUInt8(stream);

            stage.objectSpawns.emplace_back(ObjectSpawn{
                .scrollX = spriteScrollX,
                .type = spriteType,
                .spawnDelay = spriteDelay,
                .flags = spriteFlags,
                .subtype = spriteSubtype,
                .y = spriteY,
                .xrel = spriteX,
                .drop = static_cast<PowerupType>(spriteDrop),
                .minDifficulty = spriteMinDiff
            });
        }
    }

    stage.nextSpawn = stage.objectSpawns.begin();
    return stage;
}
