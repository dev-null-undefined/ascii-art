#ifndef ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24
#define ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24

#include <cstdlib>
#include <vector>
#include "sources/Color.h"
#include "sources/filters/BasicFilter.h"

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

    size_t max_depth = 5;

    Color getRoundedColor(Color color) const;

  private:
    std::vector<char> m_chars;
};

#endif //ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24
