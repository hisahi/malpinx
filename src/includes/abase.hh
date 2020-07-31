/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// abase.hh: header file for base audio library

#ifndef M_ABASE_HH
#define M_ABASE_HH

#include <vector>
#include <string>
#include "defs.hh"

// guaranteed to be called after vbase_init
void abase_init(int sampleRate);
// guaranteed to be called before vbase_quit
void abase_quit();
// whether sound system is ready
bool abase_ready();

// loopCount = 0 for infinite
void abase_music_play(const std::string &file, int loopCount);
void abase_music_fadeout(int fadeMs = S_FADEOUT_MS);
void abase_music_stop();
void abase_pause();
void abase_resume();

// data = 8-bit signed mono at given samplerate
int abase_sound_load(const std::vector<char> &data, int sampleRate);
// channel = -1 for "first free"
// playCount = 0 for infinite, 1 for 1 play, etc.
int abase_sound_play(int soundNum, float volume, float pan,
                    int playCount, int channel);
void abase_sound_fadeout(int channel, int fadeMs = S_FADEOUT_MS);
void abase_sound_stop(int channel);
void abase_sound_fadeout_all(int fadeMs = S_FADEOUT_MS);
void abase_sound_stop_all();

#endif // M_ABASE_HH
