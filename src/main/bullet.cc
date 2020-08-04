/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// bullet.cc: bullet object

#include <random>
#include "fix.hh"
#include "stage.hh"
#include "bullet.hh"

static std::random_device rngdev;
static std::mt19937 rngengine(rngdev());

static inline int randomNumber(int a, int b)
{
    return (std::uniform_int_distribution<std::mt19937::result_type>(a, b))
                                                                (rngengine);
}

BulletSprite::BulletSprite(int id, int x, int y, BulletType type)
    : Sprite(id, nullptr, x, y, SPRITE_NOSCROLL
        | SPRITE_COLLIDE_SPRITES | SPRITE_COLLIDE_FG), _type(type),
      _fx(x * Fix::ONE + Fix::HALF), _fy(y * Fix::ONE + Fix::HALF), 
      _dx(0), _dy(0), _height(16)
{
    if (_img)
    {
        _height = _img->height();
    }
}

void BulletSprite::tick()
{
    _fx += _dx;
    _fy += _dy;
    _x = static_cast<int>(_fx);
    _y = static_cast<int>(_fy);
    if (_x >= S_WIDTH || _y > S_HEIGHT || _y < 32 - _height)
    {
        kill();
        return;
    }
    // collision...
}
