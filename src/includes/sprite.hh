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
#include <iterator>
#include "defs.hh"
#include "maths.hh"
#include "render.hh"
#include "image.hh"

constexpr int SPRITE_DEFAULT = 0;
constexpr int SPRITE_NODRAW = 1;
constexpr int SPRITE_COLLIDE_SPRITES = 2;
constexpr int SPRITE_COLLIDE_FG = 4;
constexpr int SPRITE_SURVIVE_OFF_SCREEN = 8;
constexpr int SPRITE_NOSCROLL = 16;
constexpr int SPRITE_TRACKABLE = 32;

extern int colGridHeight;

// from layer.hh
class ForegroundLayer;
struct LayerScroll;

enum class SpriteType
{
    Other,
    Player,
    Drone,
    Enemy,
    Pickup,
    Boss,
    Explosion,
    Bullet,
    Script,
    Delay,
    Temporary
};

// unpacked 16-bit sprite
class Sprite
{
public:
    Sprite(int id, std::shared_ptr<Image> img, int x, int y, int flags,
            SpriteType type);
    void blit(Image &fb) const;
    void blit(Image &fb, int xoff, int yoff) const;
    int x() const { return _x; }
    int y() const { return _y; }
    int width() const { return _width; }
    int height() const { return _height; }
    int flags() const { return _flags; }
    SpriteType type() const { return _type; }
    inline bool hasFlag(int flag) const
    {
        return static_cast<bool>(_flags & flag);
    }
    void computeCollisionGrid();
    void updateImage(const std::shared_ptr<Image>& img);
    bool hitsForeground(ForegroundLayer &layer, LayerScroll scroll);
    bool boxCheck(Sprite &other);
    bool pixelCheck(Sprite &other);
    bool isDead() { return _dead; }
    void kill() { _dead = true; }
    int offScreenDistance() { return -(_x + _width); }
    void addFlags(int flag) { _flags |= flag; }
    void removeFlags(int flag) { _flags &= ~flag; }
    virtual void tick() { }
    // returns whether dead
    virtual bool damage(int dmg) { return false; }
    void move(int x, int y) { _x += x; _y += y; }
    
    inline bool fastHitCheck(Sprite &other)
    { 
        return static_cast<bool>(_colgrid & other._colgrid);
    }
    inline bool hits(Sprite &other)
    {
        return fastHitCheck(other) && boxCheck(other) && pixelCheck(other);
    }
    inline bool hits(Sprite *other)
    {
        return other && hits(*other);
    }
    template <class T>
    inline bool hits(const std::unique_ptr<T> &other)
    {
        static_assert(std::is_base_of<Sprite, T>::value, "must be a sprite");
        return other && hits(*other);
    }
protected:
    std::shared_ptr<Image> _img;
    int _id;
    int _x;
    int _y;
    int _width;
    int _height;
    int _flags;
    int _colgrid;
    int _ticks;
    SpriteType _type;
    Color _flash;
    bool _dead;
};

class Spritesheet
{
public:
    Spritesheet();
    Spritesheet(const std::vector<std::shared_ptr<Image>> &images);
    std::shared_ptr<Image> getImage(int index) const;
    void pageIn(std::shared_ptr<Image> img);
    void blit(Image &fb, int index, int x, int y) const;
    void blitFast(Image &fb, int index, int x, int y) const;

    template <class T, class... Args>
    T makeSprite(int id, int spriteIndex, int x, int y,
            int flags, Args... args) const
    {
        static_assert(std::is_base_of<Sprite, T>::value,
                                    "must be sprite type");
        return T(id, sprites.at(spriteIndex), x, y,
                                    flags, std::forward<Args>(args)...);
    }
    template <class T, class... Args>
    std::unique_ptr<T> makeUniqueSprite(int id, int spriteIndex, int x, int y,
            int flags, Args... args) const
    {
        static_assert(std::is_base_of<Sprite, T>::value,
                                    "must be sprite type");
        return std::make_unique<T>(id, sprites.at(spriteIndex), x, y,
                                    flags, std::forward<Args>(args)...);
    }
private:
    std::vector<std::shared_ptr<Image>> sprites;
};

#endif // M_SPRITE_HH
