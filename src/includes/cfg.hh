/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// cfg.hh: includes for cfg config format

#ifndef M_CFG_HH
#define M_CFG_HH

#include <iostream>
#include <string>
#include <map>

class ConfigFile
{
public:
    ConfigFile() : keys() {}
    ConfigFile(std::istream &stream);
    void write(std::ostream &stream);
    bool get(const std::string &key, bool fallback);
    int get(const std::string &key, int fallback);
    double get(const std::string &key, double fallback);
    std::string get(const std::string &key, std::string fallback);
    void set(const std::string &key, bool value);
    void set(const std::string &key, int value);
    void set(const std::string &key, double value);
    void set(const std::string &key, const std::string &value);
    void reset();
private:
    std::map<std::string, std::string> keys;
};

#endif // M_CFG_HH
