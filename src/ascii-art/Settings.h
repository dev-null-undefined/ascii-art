#ifndef ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24
#define ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24

#include <cstdlib>
#include <vector>
#include <map>
#include "sources/Color.h"
#include "sources/filters/BasicFilter.h"

/**
 * @brief ConfigKey enum
 * @details enum for possible values to be configured
 */
enum class ConfigKey: int {
    NONE = -1,
    RECURSIVE_DEPTH = 0,
    RESIZE_TIMEOUT = 1,
    CHARS = 2,
    COLOR = 3,
    DITHERING = 4,
    IMAGE_SCALE_FACTOR = 5,
};

/**
 * @brief Settings class.
 * @details class for storing, parsing, managing and loading settings.
 */
class Settings {
  public:
    /**
     * @brief Settings constructor.
     * @details constructors Settings class with default values
     * and tries to load settings from files based on the CONFIG_FILE_PATHS.
     * Reading in reversed order to allow overriding of settings from
     * files with higher priority.
     * If key provide multiple times, the last value is used.
     */
    Settings();

    /**
     * Applies filters and returns the resulting sum of the color components.
     * @param color Color to apply filters to.
     * @return sum of all Color components after applying Filters.
     */
    double colorValue(const Color & color) const;

    /**
     * Get color after color Filter is applied.
     * @param color Color to apply Filter to.
     * @return color after applying Filter.
     */
    Color modifiedColor(const Color & color) const;

    /**
     * Maximum possible that could be returned by Settings::colorValue(const Color &).
     * @return maximum possible value.
     */
    double maxValue() const;

    /**
     * Calculates closes char to give color based on the Settings::maxValue() and Settings::colorValue(const Color &).
     * @param color Color to approximate.
     * @return char that approximates the color.
     */
    char getChar(const Color & color) const;

    /**
     * Dithering and color dithering.
     * Color dithering is applied only if dithering is enabled.
     */
    bool m_dithering = true;
    bool m_color_dithering = true;

    /**
     * Color settings.
     */
    bool m_colors = true;
    bool m_debug = false;

    // TODO: should be base class instead of child
    /**
     * Brightness filter used for calculating brightness of color based on multiplicative constants.
     */
    BasicFilter m_brightness_filter{0.212671f, 0.715160f, 0.072169f, 0.0f};
    /**
     * Color filter.
     */
    BasicFilter m_color_filter{};

    /**
     * Maximum recursion depth when searching for files.
     */
    size_t m_max_depth = 2;
    /**
     * Minimum timeout between resizing.
     */
    long m_resize_timeout = 200;
    double m_image_scale_factor = 2.0;

    /**
     * Get rounded color that would be exactly equivalent to char returned by Settings::getChar(const Color &).
     * @param color rounded color.
     */
    Color getRoundedColor(Color color) const;

    /**
     * Get ConfigKey closest to the given string.
     * if none key is found or multiple keys are matching ConfigKey::NONE is returned.
     * @param key string to match.
     * @return ConfigKey closest to the given string. or ConfigKey::NONE.
     */
    static ConfigKey getConfigKey(const std::string & key);

    /**
     * Parse key-value pair based on the ConfigKey key provided.
     * if valid key-value pair is provided, the value is stored in the this Settings object.
     * @param key ConfigKey.
     * @param value Value to set to the key.
     * @return status of the operation. true if key-value pair is valid and was stored in this Settings object.
     */
    bool parseConfigField(ConfigKey key, const std::string & value);

    /**
     * Paths for config files. Last having lowest priority.
     */
    static constexpr std::string_view CONFIG_FILE_PATHS[] = { "ascii-art.conf", "~/.config/ascii-art/ascii-art.conf", "/etc/ascii-art/ascii-art.conf" };

    /**
     * Map mapping strings to ConfigKeys.
     * Used for parsing config files. Or getting string from ConfigKey enum.
     */
    static const std::map<std::string, ConfigKey> CONFIG_KEYS;

  private:
    /**
     * Chars used for ascii-art output.
     */
    std::vector<char> m_chars;

    /**
     * Read line by line file and try to parse key-value pair.
     * Separated by CONFIG_SEPARATOR.
     * @param path file read and parse.
     */
    void parseConfigFile(const std::string_view & path);

    /**
     * Separator for key-value pair.
     */
    static constexpr char CONFIG_SEPARATOR = '=';

};

/**
 * to_string method for ConfigKey enum.
 * @param key ConfigKey to convert to string.
 * @return string representation of the ConfigKey.
 */
std::string to_string(ConfigKey key);

#endif //ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24
