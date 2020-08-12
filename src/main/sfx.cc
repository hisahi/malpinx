/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sfx.cc: code for playing sound effects

#include <vector>
#include "songs.hh"
#include "sfx.hh"
#include "abase.hh"
#include "config.hh"

int PlaySound(int soundNum, float volume /*= 1.0f*/, float pan /*= 0.0f*/,
            int playCount /*= 1*/)
{
    if (!sfxEnabled) return 0;
    return abase_sound_play(soundNum, volume, pan, playCount, -1);
}

int PlaySound(SoundEffect effect, float volume /*= 1.0f*/,
            float pan /*= 0.0f*/, int playCount /*= 1*/)
{
    return PlaySound(static_cast<int>(effect), volume, pan, playCount);
}

void PlayMenuSound(int soundNum,
            float volume /*= 1.0f*/, float pan /*= 0.0f*/)
{
    if (!sfxEnabled) return;
    abase_sound_play(soundNum, volume, pan, 1, 0);
}

void PlayMenuSound(SoundEffect effect,
            float volume /*= 1.0f*/, float pan /*= 0.0f*/)
{
    PlayMenuSound(static_cast<int>(effect), volume, pan);
}

void PlayGunSound(int soundNum,
            float volume /*= 1.0f*/, float pan /*= 0.0f*/)
{
    if (!sfxEnabled) return;
    abase_sound_play(soundNum, volume, pan, 1, 1);
}

void PlayGunSound(SoundEffect effect,
            float volume /*= 1.0f*/, float pan /*= 0.0f*/)
{
    PlayGunSound(static_cast<int>(effect), volume, pan);
}

void StopSounds()
{
    abase_sound_stop_all();
}
