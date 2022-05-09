#ifndef ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24
#define ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24

#include <cstdlib>
#include <vector>
#include "sources/Color.h"

class Settings {
  public:
    Settings();

    double colorValue(const Color & color) const;

    double maxValue() const;

    char getChar(const Color & color) const;

    bool m_dithering = true;
    bool m_colors = true;
    bool m_color_dithering = true;

    double m_red_factor = 0.2126f * 100;
    double m_green_factor = 0.7152f * 100;
    double m_blue_factor = 0.0722f * 100;
    double m_alpha_factor = 0.0f * 100;

    double m_red_offset = 0.0f;
    double m_green_offset = 0.0f;
    double m_blue_offset = 0.0f;
    double m_alpha_offset = 255.0f;

    Color getRoundedColor(Color color) const;

  private:
    std::vector<char> m_chars;
};

#endif //ASCII_ART_SETTINGS_H_FE39E6A3F5334F97BCC73DCA7EBADD24
