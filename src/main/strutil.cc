/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// strutil.cc: string utility functions

#include "strutil.hh"

std::string leftAlignPad(std::string str, int n)
{
    str.insert(str.length(), n - str.length(), ' ');
    return str;
}

std::string rightAlignPad(std::string str, int n)
{
    str.insert(0, n - str.length(), ' ');
    return str;
}
