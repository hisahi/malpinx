/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// script.cc: game scripts

#include <stdexcept>
#include "object.hh"
#include "sfx.hh"
#include "songs.hh"

ScriptSprite::ScriptSprite(Shooter &stg, int id, int scriptNo)
    : Sprite(id, nullptr, 0_x, 0_x, SPRITE_NODRAW | SPRITE_NOSCROLL,
        SpriteType::Script), _script(scriptNo), _stg(stg)
{
}

void ScriptSprite::tick()
{
    ++_ticks;
    switch (_script)
    {
    case 1:
        _stg.endStage();
        kill();
        return;
    case 2:
        FadeOutSong();
        PlaySound(SoundEffect::BossAlarm, 1.0f, 0.0f, 6);
        kill();
        return;
    default:
        kill();
    }
}
