/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// gamedata.cc: code for handling game data directory, etc.

#include <fstream>
#include <stdexcept>
#include "gamedata.hh"

void CanonicalizePath(std::string &path)
{

}

std::ifstream OpenDataFile(const std::string& path)
{
    std::string xpath = "data/";
    xpath += path;
    CanonicalizePath(xpath);
    return std::ifstream(xpath);
}

void OpenDataDir()
{
    if (OpenDataFile("logo.dfp").fail())
        throw std::runtime_error("no data directory");
}

// load in as little as we can get away with
void PageInBaseData()
{
}
