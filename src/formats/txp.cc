/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// txp.cc: code for loading *.txp files

#include <fstream>
#include <string>
#include "formats.hh"
#include "gamedata.hh"

const unsigned char txp_enc[379] = {
    0xaf, 0x2b, 0x3c, 0xc1, 0xf3, 0xb7, 0x2c, 0x2a, 0x83, 0xad,
    0x27, 0xbf, 0x86, 0xc7, 0x58, 0x44, 0xff, 0xff, 0x83, 0xf8,
    0xbf, 0xea, 0x6e, 0x55, 0x2c, 0xff, 0xc8, 0x3d, 0xfe, 0xf4,
    0x6d, 0x5a, 0xca, 0x12, 0xd6, 0x7b, 0x0c, 0xe1, 0x94, 0x52,
    0x53, 0x8d, 0xec, 0x4b, 0xa5, 0x38, 0x40, 0x1f, 0x91, 0x46,
    0x0b, 0xd1, 0x1b, 0x72, 0x00, 0x81, 0x61, 0x76, 0x89, 0x01,
    0x1d, 0x62, 0x6e, 0xf8, 0x9c, 0x32, 0x01, 0xe0, 0x60, 0xcb,
    0xcc, 0x42, 0xbf, 0x10, 0xba, 0x40, 0xf6, 0x82, 0xe1, 0xa1,
    0xdf, 0x58, 0x5c, 0x20, 0xff, 0xa4, 0x9a, 0xa9, 0x83, 0xf9,
    0x58, 0x83, 0xc8, 0xbd, 0xc2, 0x9f, 0x87, 0xd6, 0x21, 0xf8,
    0x36, 0xe4, 0xb6, 0xad, 0x37, 0xda, 0xc5, 0x2d, 0x18, 0xdf,
    0x29, 0x54, 0x4b, 0x41, 0x8d, 0xd7, 0xb2, 0xf2, 0xdb, 0x7e,
    0xc4, 0xda, 0xa5, 0xa7, 0x53, 0x1f, 0xe4, 0x88, 0x63, 0x4d,
    0x9c, 0xfe, 0xd6, 0xfc, 0xbd, 0x79, 0x41, 0x3f, 0xe1, 0x48,
    0xbf, 0xb3, 0xdc, 0x98, 0xbf, 0x69, 0x8a, 0x1d, 0xa4, 0x42,
    0x32, 0x59, 0xa0, 0xf6, 0x5e, 0xd1, 0x51, 0xcb, 0x1b, 0x40,
    0x98, 0x2e, 0x38, 0x9a, 0x55, 0x1e, 0xf9, 0x15, 0xc1, 0x03,
    0x13, 0x25, 0x8f, 0xe6, 0xa5, 0x8d, 0x27, 0x63, 0x2b, 0xe1,
    0xef, 0x1f, 0x0c, 0x76, 0x0a, 0xed, 0x8e, 0x22, 0x07, 0x13,
    0xba, 0x93, 0x83, 0x3b, 0x4a, 0xf4, 0xad, 0x73, 0xff, 0x66,
    0x94, 0xbf, 0xd4, 0x7d, 0x6b, 0x0c, 0x6b, 0xb7, 0xa1, 0xba,
    0x05, 0x2c, 0x4f, 0x24, 0xc2, 0x13, 0x08, 0x4d, 0xb2, 0x6d,
    0x0b, 0x47, 0x40, 0x74, 0x5b, 0xf0, 0x38, 0x51, 0x4c, 0x4f,
    0x64, 0x42, 0x82, 0xf9, 0x9a, 0x7c, 0x6b, 0xfc, 0xeb, 0x44,
    0xa2, 0x91, 0xb4, 0x69, 0xa5, 0xc7, 0xa6, 0xe4, 0x31, 0x49,
    0xb1, 0xe7, 0x4f, 0xe0, 0xb8, 0x18, 0x25, 0xa6, 0xe5, 0x8c,
    0x77, 0x99, 0xe4, 0x9e, 0x77, 0xed, 0xca, 0x1f, 0x54, 0x32,
    0x4e, 0x01, 0x69, 0x23, 0xd1, 0xd2, 0x9c, 0x60, 0x95, 0x9a,
    0x70, 0x62, 0xcd, 0x14, 0xec, 0x5f, 0x94, 0xe3, 0xe8, 0xe7,
    0x66, 0x2b, 0xc1, 0xd8, 0xa6, 0x56, 0x3d, 0xaf, 0x33, 0xbc,
    0xa0, 0x4b, 0x74, 0xab, 0x1d, 0x4f, 0x20, 0x99, 0xaa, 0x29,
    0x25, 0xd3, 0x23, 0xc2, 0xb1, 0xd1, 0x1d, 0x35, 0xc1, 0x19,
    0x7b, 0xb9, 0xa7, 0x3a, 0x33, 0x58, 0xb8, 0x9e, 0xf0, 0xbc,
    0xe7, 0xb6, 0x1a, 0x1e, 0x2b, 0x7e, 0x57, 0xad, 0x19, 0x62,
    0xea, 0x33, 0x88, 0x6a, 0x96, 0x70, 0x1d, 0x0b, 0x57, 0x8b,
    0x0d, 0xff, 0x2a, 0xd8, 0xec, 0x16, 0x83, 0x6b, 0x32, 0xa4,
    0xa3, 0x3b, 0x4b, 0xc1, 0x41, 0xae, 0xf8, 0x5a, 0xd3, 0xc8,
    0x9a, 0x8e, 0x23, 0x5f, 0x34, 0x1e, 0x6a, 0x0b, 0x1d
};

std::string LoadTXP(const std::string &path)
{
    std::string result = "";
    auto stream = OpenDataFile(path + ".txp");
    if (stream.fail())
        return nullptr;

    int i;
    char c;
    while (stream.get(c))
    {
        result += txp_enc[i % 379] ^ (0xFF & ((c ^ i) + (i >> 8)));
        ++i;
    }

    return result;
}
