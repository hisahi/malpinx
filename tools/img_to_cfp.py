import sys
import struct
import array
from PIL import Image, ImageFile
import tip


def encode_rle(data):
    result = []
    i, last_color, rle_seq, nrle_seq, nrle_start = 0, 0, 0, 0, 0
    rle = False
    colors = list(data) + [None]

    while i < len(colors):
        color = colors[i]
        if rle:
            if color is None or color != last_color or rle_seq == 127:
                result.append((0x7F + rle_seq, None, None))
                if color is None:
                    break
                if color != last_color:
                    rle = False
                    nrle_start, nrle_seq, last_color = i, 0, None
                else:
                    rle_seq = 0
                continue
            else:
                rle_seq += 1
                i += 1
        else:
            if color is None or color == last_color or nrle_seq == 127:
                if nrle_seq > 0:
                    result.append(
                        (nrle_seq - 1, nrle_start, nrle_start + nrle_seq))
                if color == last_color:
                    rle, rle_seq = True, 1
                nrle_start, nrle_seq = i, 1
            else:
                nrle_seq += 1
            last_color = color
            i += 1

    return result


def main(*argv):
    if len(argv) <= 2:
        print(argv[0], "<input_image>", "<output_filename>")
        return 2

    source_image = Image.open(argv[1]).convert("RGBA")
    source_pixels = source_image.getdata()

    pixarray = array.array('H')
    for pixel in source_pixels:
        pixarray.append(tip.convert_color(*pixel))

    with open(argv[2], "wb") as dest_file:
        dest_file.write(struct.pack(
            "<HH", source_image.width, source_image.height))
        if sys.byteorder != 'little':
            pixarray.byteswap()

        rle = encode_rle(pixarray)
        for header, start, end in rle:
            dest_file.write(bytes([header]))
            if header < 0x80:
                pixarray[start:end].tofile(dest_file)
        dest_file.write(bytes([0xFF]))

    return 0


if __name__ == "__main__":
    sys.exit(main(*sys.argv) or 0)
