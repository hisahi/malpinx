/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// gamedata.hh: includes for gamedata.cc

#ifndef M_GAMEDATA_HH
#define M_GAMEDATA_HH

#include <fstream>
#include "defs.hh"

#define DIR_SEPARATOR "/"

std::string GetDataPath(const std::string &path);
std::ifstream OpenDataFile(const std::string &path);
void OpenDataDir();
void PageInBaseData();

#endif // M_GAMEDATA_HH
