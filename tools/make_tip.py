import sys
import os
import struct
import array
from PIL import Image, ImageFile
import tip


def main(*argv):
    if len(argv) <= 2:
        print(argv[0], '<input_sprite_list>', '<output_tip_or_tsp>')
        print('sprite list should contain lines, which have one of (each)')
        print('     filename.png')
        print('     filename.png:left,top,width,height')
        return 2

    tsp = argv[2].lower().endswith('.tsp')
    open_files = {}
    tasks = []
    is_image = os.path.splitext(argv[1])[1].lower() in ['.png']
    cwd = os.getcwd()

    if is_image:
        tasks.append((argv[1], None))
    else:
        with open(argv[1], 'r') as f:
            for line in f:
                line = line.strip()
                file, region = line, None
                if ':' in line:
                    tok = line.split(':', 1)
                    file, region = (tok[0],
                            tuple(int(i) for i in tok[1].split(',')))
                    assert len(region) == 4
                tasks.append((file, region))
        os.chdir(os.path.dirname(os.path.abspath(argv[1])))

    images = tip.apportion_images(tasks)
    assert len(images) < 65536

    if tsp:
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

    os.chdir(cwd)
    with open(argv[2], 'wb') as f:
        f.write(struct.pack('<H', len(images)))
        if tsp:
            tip.write_tip_palette(f, palette)
        for image in images:
            tip.write_tip_sprite(f, image)
    print(len(images))

    return 0


if __name__ == '__main__':
    sys.exit(main(*sys.argv) or 0)
