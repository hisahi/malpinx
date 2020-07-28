/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// colmath.cc: color math

#include "render.hh"
#include "maths.hh"

Color operator+(const Color& lhs, const Color &rhs)
{
    return Color(lhs.getR() + rhs.getR(),
                lhs.getG() + rhs.getG(),
                lhs.getB() + rhs.getB());
}

Color operator-(const Color& lhs, const Color &rhs)
{
    return Color(lhs.getR() - rhs.getR(),
                lhs.getG() - rhs.getG(),
                lhs.getB() - rhs.getB());
}
