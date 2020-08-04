/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// tiled.hh: includes for tiled.cc

#ifndef M_TILED_HH
#define M_TILED_HH

#include <cstdint>
#include "layer.hh"

using Tile = std::uint16_t;
constexpr int TILE_WIDTH = 16;
constexpr int TILE_HEIGHT = 16;
constexpr int TILEMAP_WIDTH = S_WIDTH / TILE_WIDTH;

struct Tilemap
{
    int width;
    int height;
    std::vector<Tile> tiles;
    const Tile &operator[](int index) const { return tiles[index]; }
};

// background layer made up of tiles
class BackgroundTileLayer : public BackgroundLayer
{
public:
    BackgroundTileLayer(std::shared_ptr<Spritesheet> tiles,
                        std::shared_ptr<Tilemap> map,
                        int ox, int oy, Fix sxm, Fix sym);
    void blit(Image &fb, LayerScroll scroll);
private:
    std::shared_ptr<Spritesheet> _tiles;
    std::shared_ptr<Tilemap> _map;
    int _leftmostColumn;
    int _rightmostColumn;
    int _scan;
};

// foreground layer made up of tiles
class ForegroundTileLayer : public ForegroundLayer
{
public:
    ForegroundTileLayer(std::shared_ptr<Spritesheet> tiles,
                        std::shared_ptr<Tilemap> map,
                        int ox, int oy, Fix sxm, Fix sym);
    void blit(Image &fb, LayerScroll scroll);
    inline bool hitsSprite(Image &spriteImage, LayerScroll scroll,
                int spriteX, int spriteY) const
    {
        return _img->overlapsTiled(spriteImage,
                            scroll.x + spriteX, scroll.y + spriteY,
                            0, 0,
                            spriteImage.width(), spriteImage.height());
    }
private:
    std::shared_ptr<Spritesheet> _tiles;
    std::shared_ptr<Tilemap> _map;
    int _leftmostColumn;
    int _rightmostColumn;
    int _scan;
};

#endif // M_TILED_HH
