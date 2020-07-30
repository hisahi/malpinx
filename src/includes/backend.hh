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
        return vbase_loop();
    }

    void blit() const
    {
        vbase_flip();
        vbase_sync();
    }

    int scale() const { return _scale; }
    void setScale(int s)
    {
        vbase_set_scale(_scale = s);
    }

private:
    int _scale;
};

#endif // M_BACKEND_HH
