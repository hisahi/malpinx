/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// stream.cc: code for loading values from streams

#include <fstream>
#include "gamedata.hh"
#include "formats.hh"

void ReadUInt8(std::istream &stream, std::uint8_t &value)
{
    value = static_cast<std::uint8_t>(stream.get());
}

void ReadUInt16(std::istream &stream, std::uint16_t &value)
{
    value = static_cast<std::uint16_t>(stream.get());
    value |= static_cast<std::uint16_t>(stream.get()) << 8;
}

void ReadUInt32(std::istream &stream, std::uint32_t &value)
{
    value = static_cast<std::uint32_t>(stream.get());
    value |= static_cast<std::uint32_t>(stream.get()) << 8;
    value |= static_cast<std::uint32_t>(stream.get()) << 16;
    value |= static_cast<std::uint32_t>(stream.get()) << 24;
}

void ReadInt32(std::istream &stream, std::int32_t &value)
{
    value = static_cast<std::int32_t>(stream.get());
    value |= static_cast<std::int32_t>(stream.get()) << 8;
    value |= static_cast<std::int32_t>(stream.get()) << 16;
    value |= static_cast<std::int32_t>(stream.get()) << 24;
}

void ReadFix(std::istream &stream, Fix &value)
{
    value = Fix::raw(ReadInt32(stream));
}

std::uint8_t ReadUInt8(std::istream &stream)
{ 
    std::uint8_t v; 
    ReadUInt8(stream, v);
    return v; 
}

std::uint16_t ReadUInt16(std::istream &stream)
{ 
    std::uint16_t v;
    ReadUInt16(stream, v);
    return v; 
}

std::uint32_t ReadUInt32(std::istream &stream)
{ 
    std::uint32_t v;
    ReadUInt32(stream, v);
    return v; 
}

std::int32_t ReadInt32(std::istream &stream)
{ 
    std::int32_t v;
    ReadInt32(stream, v);
    return v; 
}

Fix ReadFix(std::istream &stream)
{ 
    Fix v;
    ReadFix(stream, v);
    return v; 
}

void WriteUInt8(std::ostream &stream, std::uint8_t value)
{
    stream.put(static_cast<char>(value));
}

void WriteUInt32(std::ostream &stream, std::uint32_t value)
{
    stream.put(static_cast<char>(value & 0xFF));
    stream.put(static_cast<char>((value >> 8) & 0xFF));
    stream.put(static_cast<char>((value >> 16) & 0xFF));
    stream.put(static_cast<char>((value >> 24) & 0xFF));
}
