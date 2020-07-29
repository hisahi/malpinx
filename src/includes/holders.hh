/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// holders.hh: RAII classes

#ifndef M_HOLDERS_HH
#define M_HOLDERS_HH

#include "vbase.hh"
#include "abase.hh"

class GameBackend
{
public:
    GameBackend(int sampleRate) : _scale(1)
    {
        vbase_init();
        abase_init(sampleRate);
        vbase_set_scale(_scale);
    }

    ~GameBackend()
    {
        abase_quit();
        vbase_quit();
    }

    bool run() const
    {
        vbase_flip();
        return vbase_loop();
    }

    int scale() const { return _scale; }
    void setScale(int s)
    {
        vbase_set_scale(_scale = s);
    }

private:
    int _scale;
};

#endif // M_HOLDERS_HH
