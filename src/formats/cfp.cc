/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// cfp.cc: code for loading *.dfp and *.cfp files

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdint>
#include "render.hh"
#include "image.hh"
#include "gamedata.hh"
#include "formats.hh"

inline std::string PIC_build_message(const std::string& message,
    const std::string& path)
{
    std::ostringstream stream;
    stream << message << ": " << path << ".cfp";
    return stream.str();
}

Image LoadPIC(const std::string &path)
{
    auto stream = OpenDataFile(path + ".cfp");
    bool compressed = true;
    if (stream.fail())
    {
        stream = OpenDataFile(path + ".dfp");
        if (stream.fail())
            throw std::runtime_error(PIC_build_message(
                            "Cannot find image file", path));
        compressed = false;
    }
    stream.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);

    int width = ReadUInt16(stream);
    int height = ReadUInt16(stream);
    int size = width * height;
    std::vector<Color> colors;
    colors.reserve(size);
    if (!compressed)
        for (int i = 0; i < size; ++i)
            colors.push_back(Color(ReadUInt16(stream)));
    else
    {
        int n;
        std::uint8_t marker;
        std::uint16_t last = 0;
        do
        {
            ReadUInt8(stream, marker);
            if (marker < 0x80)
            {
                n = marker + 1;
                for (int i = 0; i < n; ++i)   
                    colors.push_back(Color(last = ReadUInt16(stream)));
            }
            else if (marker < 0xFF)
            {
                n = marker - 0x7F;
                for (int i = 0; i < n; ++i)   
                    colors.push_back(Color(last));
            }
        } while (marker != 0xFF);

        if (colors.size() != size)
            throw std::runtime_error("invalid compressed image");
    }
    return Image(width, height, std::move(colors));
}
