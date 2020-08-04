/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// config.cc: code for options

#include <fstream>
#include "config.hh"
#include "cfg.hh"
#include "defs.hh"
#include "malpinx.hh"
#include "input.hh"

static ConfigFile cfg;
constexpr char configFileName[] = "malpinx.cfg";
bool highQualityAudio;
int startContinues;

static void LoadConfigInternal()
{
    highQualityAudio = cfg.get("HQAudio", true);
    musicEnabled = cfg.get("Music", true);
    sfxEnabled = cfg.get("SoundEffects", true);
    difficulty = static_cast<DifficultyLevel>(cfg.get("Difficulty", 1));
    if (static_cast<int>(difficulty) > maxDifficultyLevel)
        difficulty = DifficultyLevel::NORMAL;
    pmode = static_cast<PlaybackMode>(cfg.get("Mode", 0));
    if (static_cast<int>(pmode) > maxPlaybackMode)
        pmode = PlaybackMode::NORMAL;
    startContinues = cfg.get("Continues", 3);
    ReadInputControls(cfg);   
}

void LoadConfig()
{
    std::ifstream file(configFileName);
    if (file.fail())
        cfg = ConfigFile();
    else
        cfg = ConfigFile(file);
    LoadConfigInternal();
}

void ApplySettingsToBackend()
{
    backend->scale(cfg.get("Scale", 1));
}

static void SaveConfigToFile()
{
    std::ofstream file(configFileName);
    if (!file.fail())
        cfg.write(file);
}

void RestoreConfigDefaults()
{
    cfg = ConfigFile();
    LoadConfigInternal();
    ApplySettingsToBackend();
    SaveConfig();
}

void SaveConfig()
{
    cfg.set("HQAudio", highQualityAudio);
    cfg.set("Scale", backend->scale());
    cfg.set("Difficulty", static_cast<int>(difficulty));
    cfg.set("Mode", static_cast<int>(pmode));
    cfg.set("Music", musicEnabled);
    cfg.set("SoundEffects", sfxEnabled);
    cfg.set("Continues", startContinues);
    SaveInputControls(cfg);
    SaveConfigToFile();
}

int GetConfigSampleRate()
{
    return highQualityAudio ? S_SAMPLE_RATE_HI : S_SAMPLE_RATE_LO;
}
