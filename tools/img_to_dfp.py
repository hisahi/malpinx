import sys
import struct
import array
from PIL import Image, ImageFile
import tip


def main(*argv):
    if len(argv) <= 2:
        print(argv[0], '<input_image>', '<output_filename>')
        return 2

    source_image = Image.open(argv[1]).convert('RGBA')
    source_pixels = source_image.getdata()

    pixarray = array.array('H')
    for pixel in source_pixels:
        pixarray.append(tip.convert_color(*pixel))

    with open(argv[2], 'wb') as dest_file:
        dest_file.write(struct.pack(
            '<HH', source_image.width, source_image.height))
        if sys.byteorder != 'little':
            pixarray.byteswap()
        pixarray.tofile(dest_file)

    return 0


if __name__ == '__main__':
    sys.exit(main(*sys.argv) or 0)
