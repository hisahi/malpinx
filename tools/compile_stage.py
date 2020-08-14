import sys
import os.path
import struct
import array


# layer??		b/t/f = background/terrain/foreground
	# i/t/p = (tiled) image, tilemap, (nontiled) image
	# filename,xm,ym,ox,oy
# 
# wave x,type,delay,subtype,ox,y
	# delay,subtype,ox,y
# end
# 
# sprite x,type,delay,subtype,ox,y


FIX_SHIFT = 8


def fix(x):
    try:
        return int(x) << FIX_SHIFT
    except:
        n, d = [int(t.strip()) for t in x.split('/')]
        return (n << FIX_SHIFT) // d


def main(*argv):
    if len(argv) <= 2:
        print(argv[0], '<input_level_script>', '<output_slp>')
        print('input sound list contains one file name per line')
        return 2

    stageheight, stagey = 208, (208 - 16) // 2
    layers, sprites = [], []
    in_wave, wave, wave_y = False, [], 0
    with open(argv[1], 'r') as f:
        x = 0
        for line in f:
            line = line.strip()
            if line.startswith('#'):
                continue
            elif line.startswith('layer'):
                data = bytearray()
                tok = [x.strip() for x in line.split(maxsplit = 1)[1]
                                            .split(',')]
                fn, xsm, ysm, ox, oy = tok
                xsm, ysm, ox, oy = fix(xsm), fix(ysm), int(ox), int(oy)
                layer_type = 'btf'.find(line[5])
                layer_format = 'itp'.find(line[6])
                assert layer_type >= 0 and layer_format >= 0
                data.append((layer_format << 4) | layer_type)
                data += struct.pack('B', 0)
                data += fn[:13].ljust(14, '\0').encode('ascii')
                data += struct.pack('<IIiiii', xsm, ysm, ox, oy, 0, -1)
                layers.append(data)
            elif line.startswith('sprite'):
                tok = [x.strip() for x in line.split(maxsplit = 1)[1]
                                            .split(',')]
                dx, t, delay, subtype, ox, y, *extra = [t for t in tok]
                dx, t, delay, subtype, ox, y = [int(t) for t in
                    (dx, t, delay, subtype, ox, y)]
                extra = [t.split("=") for t in extra]
                assert(all(len(x) == 2 for x in extra))
                extra = dict(extra)
                x += dx
                sprites.append((x, t, delay, subtype, ox, y, extra))
            elif line.startswith('wave') or line.startswith('rwave'):
                assert not in_wave
                in_wave = True
                tok = [x.strip() for x in line.split(maxsplit = 1)[1]
                                            .split(',')]
                dx, t, delay, subtype, ox, y, *extra = [t for t in tok]
                dx, t, delay, subtype, ox, y = [int(t) for t in
                    (dx, t, delay, subtype, ox, y)]
                extra = [t.split("=") for t in extra]
                assert(all(len(x) == 2 for x in extra))
                extra = dict(extra)
                wave_y = y if line.startswith('rwave') else 0
                x += dx
                wave = [(x, t, delay, subtype, ox, y, extra)]
            elif line.startswith('end') and in_wave:
                in_wave = False
                sprites += wave
            elif in_wave:
                tok = [x.strip() for x in line.split(',')]
                delay, subtype, ox, y, *extra = [t for t in tok]
                delay, subtype, ox, y = [int(t) for t in
                    (delay, subtype, ox, y)]
                extra = [t.split("=") for t in extra]
                assert(all(len(x) == 2 for x in extra))
                extra = dict(extra)
                last = wave[-1]
                wave.append((last[0], last[1], last[2] + delay,
                            subtype, ox, y + wave_y, extra))
            elif line:
                raise ValueError('unrecognized command')

    sprites.sort(key = lambda spr: spr[0])
    groups = []
    x = 0
    for sprite in sprites:
        if not groups or sprite[0] > x:
            x = sprite[0]
            groups.append((x, []))
        groups[-1][1].append(sprite[1:])
    groups.append((-1, []))

    #print(sprites)
    #print(groups)

    with open(argv[2], 'wb') as out:
        out.write(struct.pack('<HHHH', 3, stageheight, stagey, len(layers)))
        for layer in layers:
            out.write(layer)
        x = 0
        for group_x, sprites in groups:
            if group_x < 0:
                out.write(struct.pack('<i', group_x))
                break
            out.write(struct.pack('<I', group_x - x))
            x = group_x
            for sprite in sprites:
                t, delay, subtype, ox, oy, extra = sprite
                drop = int(extra.get('drop', 0))
                out.write(struct.pack('<BHBHHiiBBBB', t, delay & 0xFFFF,
                        delay >> 16, 0, subtype, oy, ox, drop, 0, 0, 0))
            out.write(b'\0')
        print(x)

    return 0


if __name__ == '__main__':
    sys.exit(main(*sys.argv) or 0)
