import sys
import struct
import array
from PIL import Image, ImageFile


def convert_color(red, green, blue):
    return (0x8000 | ((red & 0xF8) << 7)
            | ((green & 0xF8) << 2)
            | ((blue & 0xF8) >> 3))


def main(*argv):
    if len(argv) < 2:
        print(argv[0], "<input_image>", "<output_filename>")
        return 2

    source_image = Image.open(argv[1]).convert("RGB")
    source_pixels = source_image.getdata()

    pixarray = array.array('H')
    for pixel in source_pixels:
        pixarray.append(convert_color(*pixel))

    with open(argv[2], "wb") as dest_file:
        dest_file.write(struct.pack(
            "<HH", source_image.width, source_image.height))
        if sys.byteorder != 'little':
            pixarray.byteswap()
        pixarray.tofile(dest_file)

    return 0


if __name__ == "__main__":
    sys.exit(main(*sys.argv) or 0)
