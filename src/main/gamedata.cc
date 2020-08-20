/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// gamedata.cc: code for handling game data directory, etc.

#include <fstream>
#include <iostream>
#include <stdexcept>
#include "gamedata.hh"
#include "songs.hh"
#include "fonts.hh"
#include "formats.hh"
#include "explode.hh"

void CanonicalizePath(std::string &path)
{
    // no changes right now
}

std::string GetDataPath(const std::string &path)
{
    std::string xpath = "data/" + path;
    CanonicalizePath(xpath);
    return xpath;
}

std::ifstream OpenDataFile(const std::string &path)
{
    std::string xpath = GetDataPath(path);
    if (M_DEBUG) std::cerr << "Opening data file " << xpath << std::endl;
    auto stream = std::ifstream(xpath, std::ios::in | std::ios::binary);
    if (!stream.fail())
        stream.exceptions(std::ios::failbit | std::ios::badbit);
    return stream;
}

void OpenDataDir()
{
    if (OpenDataFile("malpinx.dat").fail())
        throw std::runtime_error("no data directory");
}

// load in as little as we can get away with
void PageInBaseData()
{
    LoadSongList();
    menuFont = LoadSpritesheet("menufont");
    hudFont = LoadSpritesheet("hudfont");
    bigFont = LoadSpritesheet("bigfont");
    thinFont = LoadSpritesheet("thinfont");
    hiscFont = LoadSpritesheet("hiscfont");
    LoadSounds("malpinx");
    explosionSprites = LoadSpritesheet("explanim");
}

static inline std::shared_ptr<Spritesheet> loadSprites(const std::string &s)
{
    return std::make_shared<Spritesheet>(LoadSpritesheet(s));
}

void ShooterAssets::load()
{
    menuSprites = loadSprites("menuspr");
    playerShip = loadSprites("ship");
    powerupSprites = loadSprites("powerups");
    bulletSprites = loadSprites("bullets");
    drone0 = loadSprites("drone0");
    drone1 = loadSprites("drone1");
    sigma = loadSprites("sigma");
    enemy01 = loadSprites("enemy01");
    enemy02 = loadSprites("enemy02");
    enemy03 = loadSprites("enemy03");
    enemy04 = loadSprites("enemy04");
    enemy05 = loadSprites("enemy05");
    enemy06 = loadSprites("enemy06");
    enemy07 = loadSprites("enemy07");
    enemy08 = loadSprites("enemy08");
    boss1a = loadSprites("boss1a");
}
