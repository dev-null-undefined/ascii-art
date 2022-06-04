#ifndef ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24
#define ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24

#include <cstdlib>
#include <vector>
#include <map>
#include "sources/Color.h"
#include "sources/filters/BasicFilter.h"

enum class ConfigKey: int {
    RECURSIVE_DEPTH = 1,
    RESIZE_TIMEOUT = 2,
    CHARS = 3,
    COLOR = 4,
    DITHERING = 5,
    NONE = 0,
};

class Settings {
  public:
    Settings();

    double colorValue(const Color & color) const;

    Color modifiedColor(const Color & color) const;

    double maxValue() const;

    char getChar(const Color & color) const;

    bool m_dithering = true;
    bool m_colors = true;
    bool m_color_dithering = true;
    bool m_debug = false;

    // TODO: should be base class instead of child
    BasicFilter m_brightness_filter{0.212671f, 0.715160f, 0.072169f, 0.0f};
    BasicFilter m_color_filter{};

    size_t m_max_depth = 2;
    long m_resize_timeout = 200;

    Color getRoundedColor(Color color) const;

    static ConfigKey getConfigKey(const std::string & key);

    bool parseConfigField(ConfigKey key, const std::string & value);

    static constexpr std::string_view CONFIG_FILE_PATHS[] = { "ascii-art.conf", "~/.config/ascii-art/ascii-art.conf", "/etc/ascii-art/ascii-art.conf" };

    static const std::map<std::string, ConfigKey> CONFIG_KEYS;

  private:
    std::vector<char> m_chars;

    void parseConfigFile(const std::string_view & path);

};

std::string to_string(ConfigKey key);

#endif //ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24
