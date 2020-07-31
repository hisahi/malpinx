import sys
import struct
import array
from PIL import Image


def convert_color(red, green, blue, alpha):
    if alpha < 128:
        return 0
    return (0x8000 | ((red & 0xF0) << 4)
            | ((green & 0xF0))
            | ((blue & 0xF0) >> 4))


def color_distance(a, b):
    ar, ag, ab = (a >> 8) & 15, (a >> 4) & 15, a & 15
    br, bg, bb = (b >> 8) & 15, (b >> 4) & 15, b & 15
    return (ar-br) * (ar-br) + (ag-bg) * (ag-bg) + (ab-bb) * (ab-bb)


def nearest_color(palette, color):
    if color in palette:
        return palette.index(color)
    return min(((i, p, color_distance(p, color))
                for i, p in enumerate(palette)), key=lambda x: x[2])[0]


def convert_tip_sprite(img):
    pixarray = array.array('H')
    pixarray.fromlist([convert_color(*p) for p in img.getdata()])
    return (img.width, img.height, pixarray)


def convert_tip_sprite16_to_8(img, palette):
    width, height, pixels = img
    clrarray = array.array('B')
    clrarray.fromlist([nearest_color(palette, p) for p in pixels])
    return (width, height, clrarray)


def write_tip_palette(file, palette):
    assert len(palette) == 256
    copy = palette[:]
    if sys.byteorder != 'little':
        copy.byteswap()
    copy.tofile(file)


def write_tip_sprite(file, img):
    width, height, data = img
    copy = data[:]
    file.write(struct.pack("<HH", width, height))
    if sys.byteorder != 'little':
        copy.byteswap()
    copy.tofile(file)


# tasks = [(image/filename, None | (left, top, width, height)), ...]
def apportion_images(tasks):
    image_cache = {}
    results = []
    for image, region in tasks:
        if isinstance(image, Image.Image):
            sprite = image
        elif isinstance(image, str):
            filename = image
            if filename not in image_cache:
                image_cache[filename] = Image.open(filename).convert("RGBA")
            sprite = image_cache[filename]
        else:
            raise ValueError("unrecognized image type")
        if region is not None:
            left, top, width, height = region
            sprite = sprite.crop((left, top, left + width, top + height))
        results.append(convert_tip_sprite(sprite))
    for img in image_cache.values():
        img.close()
    return results
