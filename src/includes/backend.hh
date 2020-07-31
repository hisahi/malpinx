/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// backend.hh: RAII class for backend

#ifndef M_BACKEND_HH
#define M_BACKEND_HH

#include "vbase.hh"
#include "abase.hh"
#include "ibase.hh"

class GameBackend
{
public:
    GameBackend(int sampleRate) : _scale(1)
    {
        vbase_init();
        vbase_set_scale(_scale);
        abase_init(sampleRate);
        ibase_init();
    }

    ~GameBackend()
    {
        ibase_quit();
        abase_quit();
        vbase_quit();
    }

    bool run() const
    {
        bool ret = vbase_loop();
        ibase_tick();
        return ret;
    }

    void blit() const
    {
        vbase_flip();
        vbase_sync();
    }

    int scale() const { return _scale; }
    void scale(int s) { vbase_set_scale(_scale = s); }

private:
    int _scale;
};

#endif // M_BACKEND_HH
