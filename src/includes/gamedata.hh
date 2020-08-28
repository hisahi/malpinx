/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// gamedata.hh: includes for gamedata.cc

#ifndef M_GAMEDATA_HH
#define M_GAMEDATA_HH

#include <memory>
#include <fstream>
#include "defs.hh"
#include "sprite.hh"

#define DIR_SEPARATOR "/"

std::string GetDataPath(const std::string &path);
std::ifstream OpenDataFile(const std::string &path);
void OpenDataDir();
void PageInBaseData();

struct ShooterAssets
{
    std::shared_ptr<Spritesheet> menuSprites;
    std::shared_ptr<Spritesheet> playerShip;
    std::shared_ptr<Spritesheet> powerupSprites;
    std::shared_ptr<Spritesheet> bulletSprites;
    std::shared_ptr<Spritesheet> sigma;
    std::shared_ptr<Spritesheet> drone0;
    std::shared_ptr<Spritesheet> drone1;
    std::shared_ptr<Spritesheet> enemy01;
    std::shared_ptr<Spritesheet> enemy02;
    std::shared_ptr<Spritesheet> enemy03;
    std::shared_ptr<Spritesheet> enemy04;
    std::shared_ptr<Spritesheet> enemy05;
    std::shared_ptr<Spritesheet> enemy06;
    std::shared_ptr<Spritesheet> enemy07;
    std::shared_ptr<Spritesheet> enemy08;
    std::shared_ptr<Spritesheet> enemy09;
    std::shared_ptr<Spritesheet> enemy10;
    std::shared_ptr<Spritesheet> enemy11;
    std::shared_ptr<Spritesheet> enemy12;
    std::shared_ptr<Spritesheet> enemy13;
    std::shared_ptr<Spritesheet> enemy14;
    std::shared_ptr<Spritesheet> boss1a;
    std::shared_ptr<Spritesheet> boss1b;

    void load();
};

#endif // M_GAMEDATA_HH
