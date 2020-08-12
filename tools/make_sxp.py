import sys
import os.path
import struct
import array
import wave
import decimal


def make_8bit_mono(frame, width, channels):
    candidates = []
    for c in range(channels):
        if width > 1:
            candidates.append(frame[((c + 1) * width) - 1])
        else:
            candidates.append(frame[c] ^ 0x80)
    return int(round(sum(candidates) / len(candidates)))


def frac(x):
    return x - int(x)


def unpack_sample(x):
    return ((256 - x) / -128) if x >= 128 else x / 128


def pack_sample(x):
    return min(255, round((x + 1) * 128)) ^ 0x80


def resample(samples, from_sr, to_sr):
    if from_sr == to_sr:
        return samples[:]
    resampled = array.array('B')

    def get_mixed_sample(pos):
        if pos == int(pos):
            return samples[int(pos)]
        m = float(frac(pos))
        pa, pb = (int(pos.to_integral_value(rounding=decimal.ROUND_FLOOR)),
                  int(pos.to_integral_value(rounding=decimal.ROUND_CEILING)))
        a, b = (unpack_sample(samples[pa]), 0 if pb >= len(
            samples) else unpack_sample(samples[pb]))
        return pack_sample(a * (1 - m) + b * m)

    delta = decimal.Decimal(from_sr) / decimal.Decimal(to_sr)
    elapsed = 0
    while elapsed < len(samples):
        resampled.append(get_mixed_sample(elapsed))
        elapsed += delta
    return resampled


def truncate(samples):
    n = len(samples) - 1
    while n > 5 and samples[n] == 0:
        n -= 1
    return samples[:n+2]


def write_resample(ofn, target_samplerate, sound_files, sound_folder):
    cwd = os.getcwd()
    with open(ofn, 'wb') as f:
        f.write(struct.pack('<HH', len(sound_files), target_samplerate))
        os.chdir(sound_folder)
        for sound_file in sound_files:
            try:
                sound = wave.open(sound_file, 'rb')
            except:
                print("While opening", sound_file)
                raise
            assert sound.getcomptype() == 'NONE'
            rsamples = array.array('B')
            while True:
                frame = sound.readframes(1)
                if not frame:
                    break
                rsamples.append(make_8bit_mono(frame, sound.getsampwidth(),
                                               sound.getnchannels()))
            samples = resample(
                rsamples, sound.getframerate(), target_samplerate)
            samples = truncate(samples)
            f.write(struct.pack('<I', len(samples)))
            samples.tofile(f)
            sound.close()
    os.chdir(cwd)


def main(*argv):
    if len(argv) <= 2:
        print(argv[0], '<input_sound_list>', '<output_sxp>')
        print('input sound list contains one file name per line')
        return 2

    with open(argv[1], 'r') as f:
        sound_files = [x.strip() for x in f]

    sef = os.path.dirname(os.path.abspath(argv[1]))
    decimal.getcontext().prec = 20
    pre, ext = os.path.splitext(argv[2])
    assert ext == '.sxp'
    write_resample(pre + '.sxq', 44100, sound_files, sef)
    write_resample(pre + '.sxp', 22050, sound_files, sef)

    return 0


if __name__ == '__main__':
    sys.exit(main(*sys.argv) or 0)
