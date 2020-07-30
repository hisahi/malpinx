/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sprite.hh: includes for sprite.cc

#ifndef M_SPRITE_HH
#define M_SPRITE_HH

#include <vector>
#include <memory>
#include "defs.hh"
#include "maths.hh"
#include "render.hh"
#include "image.hh"

constexpr int SPRITE_DEFAULT = 0;
constexpr int SPRITE_NODRAW = 1;

// unpacked 16-bit sprite
class Sprite
{
public:
    Sprite(std::shared_ptr<Image> img, int x, int y, int flags);
    void blit(Image &fb) const;
    int flags() const { return _flags; }
    bool hasFlag(int flag) const { return static_cast<bool>(_flags & flag); }
    virtual void tick() {};
private:
    std::shared_ptr<Image> _img;
    int _x;
    int _y;
    int _flags;
};

class Spritesheet
{
public:
    Spritesheet(const std::vector<std::shared_ptr<Image>> &images);
    std::shared_ptr<Image> getImage(int index) const;
    template <class T, class... Args>
    T makeSprite(int index, int x, int y, int flags, Args... args) const;
    void pageIn(std::shared_ptr<Image> img);
    void blit(Image &fb, int index, int x, int y) const;
    void blitFast(Image &fb, int index, int x, int y) const;
private:
    std::vector<std::shared_ptr<Image>> sprites;
};

#endif // M_SPRITE_HH
