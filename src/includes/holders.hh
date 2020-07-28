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
    GameBackend() : scale(1)
    {
        vbase_init();
        abase_init();
        vbase_set_scale(scale);
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

    int get_scale() const { return scale; }
    void set_scale(int s)
    {
        scale = s;
        vbase_set_scale(scale);
    }

private:
    int scale;
};

#endif // M_HOLDERS_HH
