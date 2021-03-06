/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// songs.cc: code for playing music

#include <vector>
#include "songs.hh"
#include "gamedata.hh"
#include "abase.hh"
#include "config.hh"

std::vector<std::string> songFiles;
std::vector<int> songFileLoops;
bool musicEnabled;
bool sfxEnabled;

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
    if (musicEnabled && trackIndex < songFiles.size())
        abase_music_play(GetDataPath(songFiles[trackIndex]),
                        songFileLoops[trackIndex]);
}

bool IsSongPlaying()
{
    return abase_song_playing();
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
