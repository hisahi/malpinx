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
    Small1, Small2, Medium1, Medium2, Large,
    TinyWhite, TinyRed, TinyGreen, TinyBlue, TinyPurple, TinyYellow
};

constexpr int EXPLOSION_ANIMS = 11;
constexpr std::array<int, EXPLOSION_ANIMS> EXPLOSION_FRAMES
    { 3, 3, 6, 6, 8, 2, 2, 2, 2, 2, 2 };
constexpr std::array<int, EXPLOSION_ANIMS> EXPLOSION_SPEED
    { 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

constexpr int generateExplosionFramePartialSum(const std::size_t N)
{
    return N > 0 ?
        EXPLOSION_FRAMES[N - 1] + generateExplosionFramePartialSum(N - 1) : 0;
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
    ExplosionSprite(int id, Fix x, Fix y, ExplosionSize size, bool center);
    ExplosionSprite &center();
    void tick();
private:
    int _divider;
    int _dividerInitial;
    int currentFrame;
    int lastFrame;
};

#endif // M_EXPLODE_HH
