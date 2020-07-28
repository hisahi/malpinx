/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// maths.hh: math functions header

#ifndef M_MATHS_HH
#define M_MATHS_HH

#include <algorithm>

template <class T>
const T& clamp(const T& value, const T& minimum, const T& maximum)
{
    return std::min(std::max(value, minimum), maximum);
}

#endif // M_MATHS_HH
