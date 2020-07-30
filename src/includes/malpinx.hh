/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// malpinx.hh: includes for malpinx.cc

#ifndef M_MALPINX_HH
#define M_MALPINX_HH

#include <memory>
#include "defs.hh"
#include "backend.hh"

extern std::unique_ptr<GameBackend> backend;
extern int sampleRate;

#endif // M_MALPINX_HH
