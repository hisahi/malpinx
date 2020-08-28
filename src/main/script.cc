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
#include "player.hh"

ScriptSprite::ScriptSprite(Shooter &stg, int id, int scriptNo)
    : Sprite(id, nullptr, 0_x, 0_x, SPRITE_NODRAW | SPRITE_NOSCROLL,
        SpriteType::Script), _script(scriptNo), _stg(stg)
{
}

void ScriptSprite::tick()
{
    int ticks = _ticks++;
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
    case 3:
        _stg.xSpeed = 0.75_x;
        kill();
        return;
    case 200:
        _stg.stage->hideLayer(4);
        _stg.stage->hideLayer(5);
        kill();
        return;
    case 201:
        if (ticks == 0)
        {
            _stg.stage->showLayer(4);
            _stg.stage->showLayer(5);
            _stg.xSpeed = 1_x / 4_x;
            _stg.stage->levelHeight = 256 + S_GHEIGHT;
        }
        if (_stg.minimumScrollY >= 256)
        {
            _stg.stage->hideLayer(0);
            _stg.stage->hideLayer(1);
            _stg.stage->hideLayer(2);
            _stg.stage->hideLayer(3);
            _stg.xSpeed = 3_x / 4_x;
            kill();
        }
        else
        {
            _stg.minimumScrollY += 1_x;
            _stg.maximumScrollY = std::max(_stg.maximumScrollY,
                            _stg.minimumScrollY);
            Fix osy = _stg.scroll.y;
            _stg.stage->spawnLevelY = _stg.maximumScrollY.round() + 100;
            _stg.scroll.y = std::max(_stg.scroll.y, _stg.minimumScrollY);
            if (_stg.isPlayerAlive())
                _stg.player->updateY(_stg.player->y() + (_stg.scroll.y - osy),
                    _stg.minimumScrollY, _stg.maximumScrollY + S_GHEIGHT);
        }
        return;
    case 202:
        if (ticks == 0)
        {
            _stg.stage->showLayer(0);
            _stg.stage->showLayer(1);
            _stg.stage->showLayer(2);
            _stg.stage->showLayer(3);
            _stg.xSpeed = 1_x / 4_x;
        }
        if (_stg.minimumScrollY <= 256 - S_HEIGHT)
        {
            _stg.stage->hideLayer(4);
            _stg.stage->hideLayer(5);
            _stg.xSpeed = 3_x / 4_x;
            _stg.stage->levelHeight = 256;
            kill();
        }
        else
        {
            _stg.minimumScrollY -= 1_x;
            _stg.maximumScrollY -= 1_x;
            Fix osy = _stg.scroll.y;
            _stg.stage->spawnLevelY = _stg.maximumScrollY.round() + 100;
            _stg.scroll.y = std::min(_stg.scroll.y,
                                    _stg.maximumScrollY);
            if (_stg.isPlayerAlive())
                _stg.player->updateY(_stg.player->y() + (_stg.scroll.y - osy),
                    _stg.minimumScrollY, _stg.maximumScrollY + S_GHEIGHT);
        }
        return;
    default:
        kill();
    }
}
