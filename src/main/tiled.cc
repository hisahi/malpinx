/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// tiled.cc: tiled layer implementations

#include <memory>
#include <algorithm>
#include "layer.hh"
#include "sprite.hh"
#include "tiled.hh"

static inline REALLY_INLINE void drawColumn(Image &dst, Spritesheet &tiles,
        Tilemap &map, int targetX, int columnX)
{
    int y = 0, p = map.height * columnX;
    for (int ty = 0; ty < map.height; ++ty)
    {
        tiles.blitFast(dst, map[p++], targetX, y);
        y += TILE_HEIGHT;
    }
}

BackgroundTileLayer::BackgroundTileLayer(std::shared_ptr<Spritesheet> tiles,
                std::shared_ptr<Tilemap> map, int ox, int oy, Fix sxm, Fix sym)
    : BackgroundLayer(
            std::make_shared<Image>(S_WIDTH + TILE_WIDTH,
                    map->height * TILE_HEIGHT), ox, oy, sxm, sym),
      _tiles(tiles), _map(map),
      _leftmostColumn(0), _rightmostColumn(-1)
{
}

void BackgroundTileLayer::blit(Image &fb, LayerScroll scroll)
{
    int sx = static_cast<int>(scroll.x * _scrollXMul);
    int sy = static_cast<int>(scroll.y * _scrollYMul);
    int newLeftMost = sx / TILE_WIDTH;
    int newRightMost = newLeftMost + TILEMAP_WIDTH;
    while (newRightMost > _rightmostColumn)
    {
        drawColumn(*_img, *_tiles, *_map, _scan, ++_rightmostColumn);
        _scan = (_scan + TILE_WIDTH) % (S_WIDTH + TILE_WIDTH);
        if (_rightmostColumn - _leftmostColumn > TILEMAP_WIDTH)
            ++_leftmostColumn;
    }
    _img->blitTiled(fb, 0, 0, sx - _offsetX, sy - _offsetY, S_WIDTH, S_HEIGHT);
}

ForegroundTileLayer::ForegroundTileLayer(std::shared_ptr<Spritesheet> tiles,
                std::shared_ptr<Tilemap> map, int ox, int oy, Fix sxm, Fix sym)
    : ForegroundLayer(
            std::make_shared<Image>(S_WIDTH + TILE_WIDTH,
                    map->height * TILE_HEIGHT), ox, oy, sxm, sym),
      _tiles(tiles), _map(map),
      _leftmostColumn(0), _rightmostColumn(-1)
{
}

void ForegroundTileLayer::blit(Image &fb, LayerScroll scroll)
{
    int sx = static_cast<int>(scroll.x * _scrollXMul);
    int sy = static_cast<int>(scroll.y * _scrollYMul);
    int newLeftMost = sx / TILE_WIDTH;
    int newRightMost = newLeftMost + TILEMAP_WIDTH;
    while (newRightMost > _rightmostColumn)
    {
        drawColumn(*_img, *_tiles, *_map, _scan, ++_rightmostColumn);
        _scan = (_scan + TILE_WIDTH) % (S_WIDTH + TILE_WIDTH);
        if (_rightmostColumn - _leftmostColumn > TILEMAP_WIDTH)
            ++_leftmostColumn;
    }
    _img->blitTiled(fb, 0, 0, sx - _offsetX, sy - _offsetY, S_WIDTH, S_HEIGHT);
}
