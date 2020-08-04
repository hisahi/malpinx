/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// explode.hh: include for explosion objects

#ifndef M_EXPLODE_HH
#define M_EXPLODE_HH

#include "object.hh"
#include "sprite.hh"

extern Spritesheet explosionSprites;

enum class ExplosionSize
{
    SMALL, MEDIUM, LARGE,
    TINY_WHITE
};

constexpr int EXPLOSION_FRAMES[6] = { 3, 3, 6, 6, 8, 2 };
constexpr int EXPLOSION_ANIMS = sizeof(EXPLOSION_FRAMES) / sizeof(int);

constexpr int generateExplosionFramePartialSum(const std::size_t N)
{
    return EXPLOSION_FRAMES[N]
            + (N > 0 ? generateExplosionFramePartialSum(N - 1) : 0);
}

template <std::size_t... I>
constexpr std::array<int, sizeof...(I)> generateExplosionFramesStart(
                                            std::index_sequence<I...>)
{
    return std::array<int, sizeof...(I)>{
                        generateExplosionFramePartialSum(I)... };
}

template <std::size_t N>
constexpr std::array<int, N> generateExplosionFramesStart()
{
    return generateExplosionFramesStart(std::make_index_sequence<N>());
}

constexpr std::array<int, EXPLOSION_ANIMS> EXPLOSION_FRAMES_START =
                generateExplosionFramesStart<EXPLOSION_ANIMS>();

class ExplosionSprite : public Sprite
{
public:
    ExplosionSprite(int id, int x, int y, ExplosionSize size);
    void tick();
private:
    bool _divider;
    int currentFrame;
    int lastFrame;
};

#endif // M_EXPLODE_HH
