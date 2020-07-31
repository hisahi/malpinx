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
    LoadSounds("malpinx");
}
