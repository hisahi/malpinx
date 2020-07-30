/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// songs.cc: class for playing music songs

#include <vector>
#include "songs.hh"
#include "gamedata.hh"
#include "abase.hh"

std::vector<std::string> songFiles;
std::vector<int> songFileLoops;

void LoadSongList()
{
    songFiles.clear();
    auto songList = std::ifstream(GetDataPath("malpinx.sng"));
    if (!songList.fail())
    {
        std::string line;
        int loop;
        while (std::getline(songList, line))
        {
            loop = -1;
            if (!line.empty() && line.at(0) == '!')
            {
                loop = 1;
                line = line.substr(1);
            }
            songFiles.push_back(line);
            songFileLoops.push_back(loop);
        }
    }
}

void PlaySong(MusicTrack track)
{
    int trackIndex = static_cast<int>(track);
    if (trackIndex < songFiles.size())
        abase_music_play(GetDataPath(songFiles[trackIndex]),
                        songFileLoops[trackIndex]);
}

void PauseSound()
{
    abase_pause();
}

void ResumeSound()
{
    abase_resume();
}

void FadeOutSong(int fadeMs /*= S_FADEOUT_MS*/)
{
    abase_music_fadeout(fadeMs);
}

void StopSong()
{
    abase_music_stop();
}
