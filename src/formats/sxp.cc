/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sxp.cc: code for loading *.sxp files

#include <fstream>
#include <string>
#include <iterator>
#include "formats.hh"
#include "gamedata.hh"
#include "config.hh"
#include "abase.hh"

int LoadSounds(const std::string &path)
{
    std::string result = "";
    auto stream = OpenDataFile(path + (highQualityAudio ? ".sxq" : ".sxp"));
    if (stream.fail())
        return -1;
    if (!abase_ready())
        return -1;
    stream.exceptions(std::ios::eofbit | std::ios::failbit | std::ios::badbit);
    
    int index = -1, newIndex;
    int numSounds = ReadUInt16(stream);
    int sampleRate = ReadUInt16(stream);
    std::vector<char> data;

    for (int i = 0; i < numSounds; ++i)
    {
        size_t samples = static_cast<size_t>(ReadUInt32(stream));
        data.clear();
        data.resize(samples);
        stream.read(&data[0], samples);
        newIndex = abase_sound_load(data, sampleRate);
        if (index < 0)
            index = newIndex;
    }

    return index;
}
