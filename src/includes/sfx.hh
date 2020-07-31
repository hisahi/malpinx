/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sfx.hh: includes for sfx.cc

#ifndef M_SFX_HH
#define M_SFX_HH

#include <defs.hh>

enum class SoundEffect
{
    MenuChangeOption,
    MenuSelectOption
};

int PlaySound(int soundNum, float volume = 1.0f, float pan = 0.0f,
            int playCount = 1);
int PlaySound(SoundEffect effect, float volume = 1.0f, float pan = 0.0f,
            int playCount = 1);
void PlayMenuSound(int soundNum, float volume = 1.0f, float pan = 0.0f);
void PlayMenuSound(SoundEffect effect, float volume = 1.0f, float pan = 0.0f);
void StopSounds();

#endif // M_SFX_HH
