import math
import drawSvg as draw

def scale(a, b, d):
    return (1-d)*a+d*b

# The below function is a ***modified*** function from the drawSvg library.
# 
# Copyright 2017 Casey Duckering
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
def arc2(self, cx, cy, rx, ry, startDeg, endDeg, cw=False, includeM=True,
        includeL=False):
    ''' Uses A() to draw a circular arc '''
    largeArc = (endDeg - startDeg) % 360 > 180
    startRad, endRad = startDeg*math.pi/180, endDeg*math.pi/180
    sx, sy = rx*math.cos(startRad), ry*math.sin(startRad)
    ex, ey = rx*math.cos(endRad), ry*math.sin(endRad)
    if includeL:
        self.L(cx+sx, cy+sy)
    elif includeM:
        self.M(cx+sx, cy+sy)
    return self.A(rx, ry, 0, largeArc ^ cw, cw, cx+ex, cy+ey)

def M(d, x, y, w, h):
    xb, yb = x + w, y + h
    x1, x2 = x + sw, xb - sw
    x3, x4 = x + (3/2) * sw, xb - (3/2) * sw
    x0 = scale(x, xb, 0.5)
    ym = 0.75
    y1 = scale(y, yb, ym)
    y2 = scale(y, yb, ym / 1.5)
    y3 = y2
    d.append(draw.Lines(x, yb,
                        x1, yb,
                        x1, y2,
                        x3, y1,
                        x4, y1,
                        x2, y2,
                        x2, yb,
                        xb, yb,
                        xb, y,
                        x2, y,
                        x0, y3,
                        x1, y,
                        x, y,
                        close = True, fill=color))
def A(d, x, y, w, h):
    xb, yb = x + w, y + h
    x0 = scale(x, xb, 0.5)
    x1, x4 = x + sw, xb - sw
    x2, x3 = x0 - sw / 2, x0 + sw / 2
    ym = 0.6875
    y1 = scale(y, yb, ym / 1.5 * 0.5)
    y2, y3 = scale(y, yb, ym) - sw / 2, scale(y, yb, ym) + sw / 2
    x5, x6 = scale(x, x0, ym - 0.125), scale(xb, x0, ym - 0.125)
    d.append(draw.Lines(x, yb,
                        x1, yb,
                        x0, y1,
                        x4, yb,
                        xb, yb,
                        x3, y,
                        x2, y,
                        close = True, fill=color))
    d.append(draw.Lines(x5, y2,
                        x6, y2,
                        x6, y3,
                        x5, y3,
                        close = True, fill=color))
def L(d, x, y, w, h):
    xb, yb = x + w, y + h
    x1 = x + sw
    y1 = yb + sw
    d.append(draw.Lines(x, yb,
                        xb, yb,
                        xb, y1,
                        x1, y1,
                        x1, y,
                        x, y,
                        close = True, fill=color))
def P(d, x, y, w, h):
    xb, yb = x + w, y + h
    x1 = x + sw
    y1, y2 = y - sw, scale(y, yb, 0.5)
    x2 = scale(x, xb, 0.625)
    y3 = y2 + sw
    y0 = scale(y1, y3, 0.5)
    d.append(draw.Lines(x, yb,
                        x1, yb,
                        x1, y3,
                        x2, y3,
                        x2, y2,
                        x1, y2,
                        x1, y1,
                        x2, y1,
                        x2, y,
                        x, y,
                        close = True, fill=color))
    p = draw.Path(fill=color)
    # cx, cy, rx, ry, startDeg, endDeg, cw=False, includeM=True, includeL=False
    arc2(p, x2, y0, xb - x2, abs(y - y0), 90, 270, cw=True)
    arc2(p, x2, y0, xb - x2 - sw, abs(y0 - y1), 270, 90, includeL=True)
    p.Z()
    d.append(p)
def I(d, x, y, w, h):
    xb, yb = x + w, y + h
    d.append(draw.Lines(x, y,
                        xb, y,
                        xb, yb,
                        x, yb,
                        close = True, fill=color))
def N(d, x, y, w, h):
    xb, yb = x + w, y + h
    x1, x2 = x + sw, xb - sw
    x0 = scale(x, xb, 0.5)
    ym = 0.75
    y1 = scale(y, yb, ym)
    y2 = scale(y, yb, ym / 2)
    y3 = scale(y, yb, 1 - (ym / 2))
    d.append(draw.Lines(x, yb,
                        x1, yb,
                        x1, y2,
                        x2, yb,
                        xb, yb,
                        xb, y,
                        x2, y,
                        x2, y3,
                        x1, y,
                        x, y,
                        close = True, fill=color))
def X(d, x, y, w, h):
    xb, yb = x + w, y + h
    x1, x2 = x + sw, xb - sw
    d.append(draw.Lines(x, y,
                        x1, y,
                        xb, yb,
                        x2, yb,
                        close = True, fill=color))
    d.append(draw.Lines(xb, y,
                        x2, y,
                        x, yb,
                        x1, yb,
                        close = True, fill=color))
def _(d, x, y, w, h):
    pass

letters = [M,A,L,P,I,N,X]
sw = 40
lw = [sw * 4, sw * 4, sw * 4, sw * 4, sw, sw * 4, sw * 4]
color = 'black'
ls, lh = 30, 350
tw, th = sum(lw) + ls * 6, lh
d = draw.Drawing(tw, th, origin='center', displayInline=False)
x = 0
g = draw.Group(style="""transform: matrix3d(1,0,0,0,
                                            0,1,0,-0.00125,
                                            0,0,1,0,
                                            0,0,0,1.25)""")
for letter, width in zip(letters, lw):
    letter(g, x - tw / 2, th / 2, width, -lh)
    x += width + ls
d.append(g)

d.saveSvg("../assets/malpinx.svg")

