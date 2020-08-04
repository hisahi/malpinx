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
#include "tiled.hh"

inline std::string TLP_build_message(const std::string& message,
    const std::string& path)
{
    std::ostringstream stream;
    stream << message << ": " << path << ".tlp";
    return stream.str();
}

Tilemap LoadTilemap(const std::string &path, std::string &tileSheetName)
{
    auto stream = OpenDataFile(path + ".tlp");
    bool shared = false;
    if (stream.fail())
        throw std::runtime_error(TLP_build_message(
                        "Cannot open tilemap file", path));
    stream.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
    if (ReadUInt16(stream) != 1)
        throw std::runtime_error(TLP_build_message(
                        "Cannot open tilemap file", path));
    ReadUInt8(stream);
    std::array<char, 13> nameArr;
    stream.read(&nameArr[0], 13);
    tileSheetName = std::string(nameArr.data());

    std::vector<Tile> tiles;
    int width, height, size;
    size = width * height;
    tiles.reserve(size);

    for (int i = 0; i < size; ++i)
        tiles.push_back(ReadUInt16(stream));

    return Tilemap{ .width = width, .height = height, .tiles = tiles };
}
