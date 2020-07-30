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

inline std::string TIP_build_message(const std::string& message,
    const std::string& path)
{
    std::ostringstream stream;
    stream << message << ": " << path << ".tip";
    return stream.str();
}

Spritesheet LoadSpritesheet(const std::string &path)
{
    auto stream = OpenDataFile(path + ".tip");
    bool shared = false;
    if (stream.fail())
    {
        stream = OpenDataFile(path + ".tsp");
        if (stream.fail())
            throw std::runtime_error(TIP_build_message(
                            "Cannot find spritesheet file", path));
        shared = true;
    }
    stream.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);

    std::vector<std::shared_ptr<Image>> images;
    int sprites = ReadUInt16(stream);
    int width, height, size;
    std::array<Color, 256> palette;

    if (shared)
        for (int i = 0; i < 256; ++i)
            palette[i] = Color(ReadUInt16(stream));

    for (int i = 0; i < sprites; ++i)
    {
        std::vector<Color> colors;
        width = ReadUInt16(stream);
        height = ReadUInt16(stream);
        size = width * height;
        colors.reserve(size);

        if (shared)
            for (int j = 0; j < size; ++j)
                colors.push_back(palette[ReadUInt8(stream)]);
        else
            for (int j = 0; j < size; ++j)
                colors.push_back(Color(ReadUInt16(stream)));

        images.push_back(std::make_shared<Image>(width, height,
                         std::move(colors)));
    }

    return Spritesheet(images);
}
