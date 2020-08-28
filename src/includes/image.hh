/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// image.hh: includes for image.cc; the Image class

#ifndef M_IMAGE_HH
#define M_IMAGE_HH

#include <vector>
#include <cstdint>
#include <array>
#include "defs.hh"
#include "maths.hh"
#include "color.hh"

constexpr int S_STRIDE = S_WIDTH;

class Image
{
public:
    Image(int width, int height);
    Image(int width, int height, std::vector<Color> &&data);
    void blit(Image &dst, int dx, int dy, int sx, int sy, int sw, int sh);
    void blitTiled(Image &dst, int dx, int dy, int sx, int sy, int sw, int sh);
    // ignores transparency on this (source) image
    void blitFast(Image &dst, int dx, int dy, int sx, int sy, int sw, int sh);
    void blitAdditive(Image &dst, int dx, int dy,
                        int sx, int sy, int sw, int sh);
    void blitAdditiveTiled(Image &dst, int dx, int dy,
                        int sx, int sy, int sw, int sh);
    void clear();
    void fill(Color color);
    bool overlaps(Image &other, int x, int y, int ox, int oy,
                        int w, int h) const;
    bool overlapsTiled(Image &other, int x, int y, int ox, int oy,
                        int w, int h) const;
    int width() const { return _width; }
    int height() const { return _height; }
    std::vector<Color> &buffer() { return _data; }
    void add(Color color);
    void subtract(Color color);
    void addSolid(Color color);
    void subtractSolid(Color color);
    void addSolid(Color color, int x, int y, int w, int h);
    void subtractSolid(Color color, int x, int y, int w, int h);
    
    inline void blit(Image &fb)
    {
        blit(fb, 0, 0, 0, 0, _width, _height);
    }
    inline void blit(Image &fb, int x, int y)
    {
        blit(fb, x, y, 0, 0, _width, _height);
    }
private:
    int _width;
    int _height;
    std::vector<Color> _data;
};

#endif // M_IMAGE_HH
