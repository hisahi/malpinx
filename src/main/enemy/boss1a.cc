/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// boss1a.cc: code for stage 1 miniboss

#include "enemy.hh"
#include "fix.hh"
#include "bullet.hh"

Boss1a::Boss1a(Shooter &stg, int id, Fix x, Fix y,
        int subtype, PowerupType drop)
    : BossSprite(stg, id, x, y, 1000, drop)
{
    _score = 2000;
    updateImage(_stg.assets.boss1a->getImage(0));
    _targetXScroll = _stg.scroll.x + 176_x;
}

void Boss1a::killEnemy()
{
    EnemySprite::killEnemy();
    if (_scrollStopped)
        std::swap(_stg.xSpeed, _oldXScroll);
}

void Boss1a::explode()
{
    _stg.explode(_x + _width / 2, _y + _height / 2,
                ExplosionSize::Large, false);
    _stg.explode(_x + _width / 2 - 40, _y + _height / 2,
                ExplosionSize::Large, true);
    _stg.explode(_x + _width / 2 + 40, _y + _height / 2,
                ExplosionSize::Large, true);
    killEnemy();
}

void Boss1a::doBossTick()
{
    damagePlayerOnTouch();
    if (!_scrollStopped && _stg.scroll.x >= _targetXScroll)
    {
        _scrollStopped = true;
        std::swap(_stg.xSpeed, _oldXScroll);
        _stg.scroll.x = _targetXScroll;
    }
}
