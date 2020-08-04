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
    Ending,
    NameEntry,
    Stage1,
    Stage2,
    Stage3,
    Stage4,
    Stage5,
    FinalStage,
    Boss,
    FinalBoss,
    StageClear,
    GameOver
};

void LoadSongList();
void PlaySong(MusicTrack track);
void PauseSound();
void ResumeSound();
void FadeOutSong(int fadeMs = S_FADEOUT_MS);
void StopSong();

#endif // M_SONGS_HH
