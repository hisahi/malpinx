import sys
import struct
import array
from PIL import Image, ImageFile
import tip


def main(*argv):
    if len(argv) <= 4:
        print(argv[0], '<input_image>', '<char_width>',
              '<char_height>', '<output_tip>')
        print('input image size should be a multiple'
              + 'of char_width, char_height')
        return 2

    tsp = argv[2].lower().endswith('.tsp')
    open_files = {}
    tasks = []
    cwidth, cheight = int(argv[2]), int(argv[3])

    sheet = Image.open(argv[1]).convert('RGBA')
    assert sheet.width % cwidth == 0
    assert sheet.height % cheight == 0

    for y in range(0, sheet.height, cheight):
        for x in range(0, sheet.width, cwidth):
            tasks.append((sheet, (x, y, cwidth, cheight)))

    images = tip.apportion_images(tasks)
    assert len(images) < 65536

    palette = array.array('H')
    used_colors = {}
    for image in images:
        for color in image[2]:
            used_colors[color] = used_colors.get(color, 0) + 1
    del used_colors[0]
    most_used_colors = [c[0] for c in list(sorted(
        [(color, count) for color, count in used_colors.items()],
        key=lambda item: item[1], reverse=True))[:255]]
    if len(most_used_colors) < 255:
        most_used_colors += [0 for _ in range(255 - len(most_used_colors))]
    palette.fromlist([0] + most_used_colors)
    images = [tip.convert_tip_sprite16_to_8(i, palette) for i in images]

    with open(argv[4], 'wb') as f:
        f.write(struct.pack('<H', len(images)))
        tip.write_tip_palette(f, palette)
        for image in images:
            tip.write_tip_sprite(f, image)

    return 0


if __name__ == '__main__':
    sys.exit(main(*sys.argv) or 0)
