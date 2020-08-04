/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// m_logo.hh: header for m_logo.cc

#ifndef M_MLOGO_HH
#define M_MLOGO_HH

#include "image.hh"

class LogoScreen
{
public:
    LogoScreen(int sequence, Image &&image);
    bool fadeOut();
    int sequence;
    void blit(Image &fb);
private:
    int _ticks;
    std::unique_ptr<Image> _image;
};

#endif // M_MLOGO_HH
