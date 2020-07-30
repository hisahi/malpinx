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

static ConfigFile cfg;
constexpr char configFileName[] = "malpinx.cfg";

bool highQualityAudio;

void LoadConfig()
{
    std::ifstream file(configFileName);
    if (file.fail())
        cfg = ConfigFile();
    else
        cfg = ConfigFile(file);

    highQualityAudio = cfg.get("HQAudio", true);
}

void ApplySettingsToBackend()
{
    backend->scale(cfg.get("Scale", 1));
}

void SaveConfig()
{
    std::ofstream file(configFileName);
    if (!file.fail())
        cfg.write(file);
}

int GetConfigSampleRate()
{
    return highQualityAudio ? S_SAMPLE_RATE_HI : S_SAMPLE_RATE_LO;
}