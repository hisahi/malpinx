/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// config.hh: includes for config.cc

#ifndef M_CONFIG_HH
#define M_CONFIG_HH

#include "defs.hh"
#include "modes.hh"

void LoadConfig();
void ApplySettingsToBackend();
void SaveConfig();
void RestoreConfigDefaults();
int GetConfigSampleRate();

extern bool highQualityAudio;
extern DifficultyLevel difficulty;
extern PlaybackMode pmode;
extern int startContinues;
extern bool musicEnabled;
extern bool sfxEnabled;

#endif // M_CONFIG_HH
