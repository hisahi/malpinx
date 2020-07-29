/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// config.hh: includes for config.cc

#ifndef M_CONFIG_HH
#define M_CONFIG_HH

void LoadConfig();
void ApplySettingsToBackend();
void SaveConfig();
int GetConfigSampleRate();

extern bool highQualityAudio;

#endif // M_CONFIG_HH
