/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// color.cc: implementation of Color

#include "color.hh"
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

Color& Color::operator+=(const Color& rhs)
{
    setR(getR() + rhs.getR());
    setG(getG() + rhs.getG());
    setB(getB() + rhs.getB());
    return *this;
}

Color& Color::operator-=(const Color& rhs)
{
    setR(getR() - rhs.getR());
    setG(getG() - rhs.getG());
    setB(getB() - rhs.getB());
    return *this;
}
