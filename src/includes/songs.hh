/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// songs.hh: includes for songs.cc

#ifndef M_SONGS_HH
#define M_SONGS_HH

#include <defs.hh>

enum class MusicTrack
{
    Title,
    Options,
    Cutscene,
    LevelSelect,
    Ending,
    Credits,
    NameEntry,
    Level1,
    Level2,
    Level3,
    Level4,
    Level5,
    Level6,
    Level7,
    Level8,
    Boss1,
    Boss2,
    FinalBoss,
};

void LoadSongList();
void PlaySong(MusicTrack track);
void PauseSound();
void ResumeSound();
void FadeOutSong(int fadeMs = S_FADEOUT_MS);
void StopSong();

#endif // M_SONGS_HH
