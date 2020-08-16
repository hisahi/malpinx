/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// hsc.cc: code for loading and saving *.hsc high score files

#include <vector>
#include <memory>
#include <algorithm>
#include "formats.hh"

std::vector<ScoreEntry> LoadHighScoreTable(std::istream &stream)
{
    std::vector<ScoreEntry> result;
    int numEntries = std::min(static_cast<int>(ReadUInt8(stream)),
                                HIGHSCORE_COUNTS);
    
    for (int i = 0; i < numEntries; ++i)
    {
        char a, b, c, s;
        std::uint32_t score;
        a = static_cast<char>(ReadUInt8(stream));
        b = static_cast<char>(ReadUInt8(stream));
        c = static_cast<char>(ReadUInt8(stream));
        s = static_cast<char>(ReadUInt8(stream));
        score = ReadUInt32(stream);
        result.push_back({
            .name{a, b, c},
            .stage = s,
            .score = score
        });
    }
        
    std::stable_sort(result.begin(), result.end(),
        [](const ScoreEntry &a, const ScoreEntry &b)
        {
            // intentionally in reverse
            return a.score > b.score;
        });

    return result;
}

void SaveHighScoreTable(std::ostream &stream,
    const std::vector<ScoreEntry> &entries)
{
    int numEntries = entries.size();
    if (numEntries > HIGHSCORE_COUNTS || numEntries < 0)
        numEntries = HIGHSCORE_COUNTS;
    WriteUInt8(stream, numEntries);

    for (int i = 0; i < numEntries; ++i)
    {
        ScoreEntry entry = entries[i];
        WriteUInt8(stream, entry.name[0]);
        WriteUInt8(stream, entry.name[1]);
        WriteUInt8(stream, entry.name[2]);
        WriteUInt8(stream, entry.stage);
        WriteUInt32(stream, entry.score);
    }
}
