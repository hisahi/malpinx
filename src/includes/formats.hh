/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// formats.hh: includes for format loading

#ifndef M_FORMATS_HH
#define M_FORMATS_HH

#include <cstdint>
#include <memory>
#include <iostream>
#include "image.hh"
#include "sprite.hh"
#include "m_game.hh"
#include "tiled.hh"
#include "stage.hh"
#include "fix.hh"

void ReadUInt8(std::istream &stream, std::uint8_t &value);
void ReadUInt16(std::istream &stream, std::uint16_t &value);
void ReadUInt32(std::istream &stream, std::uint32_t &value);
void ReadInt32(std::istream &stream, std::int32_t &value);
void ReadFix(std::istream &stream, Fix &value);

std::uint8_t ReadUInt8(std::istream &stream);
std::uint16_t ReadUInt16(std::istream &stream);
std::uint32_t ReadUInt32(std::istream &stream);
std::int32_t ReadInt32(std::istream &stream);
Fix ReadFix(std::istream &stream);

Image LoadPIC(const std::string &path);
std::string LoadTXP(const std::string &path);
Spritesheet LoadSpritesheet(const std::string &path);
int LoadSounds(const std::string &path);
Tilemap LoadTilemap(const std::string &path, std::string &tileSheetName);
Stage LoadStage(const std::string &path, Shooter &stg);

#endif // M_FORMATS_HH
