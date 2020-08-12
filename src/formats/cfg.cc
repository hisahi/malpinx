/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// cfg.cc: code for cfg config file handling

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "cfg.hh"

ConfigFile::ConfigFile(std::istream &stream) : ConfigFile()
{
    std::string line, key, value;
    int pos;
    while (std::getline(stream, line))
    {
        if (line.empty() || line.at(0) == '#')
            continue;
        pos = line.find("=");
        if (pos < 0)
            continue;
        key = line.substr(0, pos);
        value = line.substr(pos + 1);
        keys.insert(std::pair<std::string, std::string>(key, value));
    }
}

void ConfigFile::write(std::ostream &stream)
{
    for (auto const& pair : keys)
        stream << pair.first << "=" << pair.second << std::endl;
}

bool ConfigFile::get(const std::string &key, bool fallback)
{
    auto it = keys.find(key);
    if (it == keys.end())
        set(key, fallback);
    bool b;
    std::istringstream(keys[key]) >> std::boolalpha >> b;
    return b;
}

int ConfigFile::get(const std::string &key, int fallback)
{
    auto it = keys.find(key);
    if (it == keys.end())
        set(key, fallback);
    return std::stoi(keys[key]);
}

double ConfigFile::get(const std::string &key, double fallback)
{
    auto it = keys.find(key);
    if (it == keys.end())
        set(key, fallback);
    return std::stod(keys[key]);
}

std::string ConfigFile::get(const std::string &key, std::string fallback)
{
    auto it = keys.find(key);
    if (it == keys.end())
        set(key, fallback);
    return keys[key];
}

void ConfigFile::set(const std::string &key, bool value)
{
    keys[key] = value ? "true" : "false";
}

void ConfigFile::set(const std::string &key, int value)
{
    keys[key] = std::to_string(value);
}

void ConfigFile::set(const std::string &key, double value)
{
    keys[key] = std::to_string(value);
}

void ConfigFile::set(const std::string &key, const std::string &value)
{
    keys[key] = value;
}

void ConfigFile::reset()
{
    keys.clear();
}
