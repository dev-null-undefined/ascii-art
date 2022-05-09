#include <string>
#include "Settings.h"

double Settings::maxValue() const {
    return colorValue(Color{255, 255, 255, 0});
}

double Settings::colorValue(const Color & color) const {
    double value = 0;
    value += (color.getRed() + m_red_offset) * m_red_factor;
    value += (color.getGreen() + m_green_offset) * m_green_factor;
    value += (color.getBlue() + m_blue_offset) * m_blue_factor;
    value += (color.getAlpha() + m_alpha_offset) * m_alpha_factor;
    return value;
}

Settings::Settings() {
    std::string chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    m_chars = std::vector<char>(chars.begin(), chars.end());
}

char Settings::getChar(const Color & color) const {
    double value = colorValue(color);
    double max = maxValue();
    auto index = (size_t) (value * (double) (m_chars.size() - 1) / max);
//#ifdef DEBUG // TODO: fix this lazy hack
    if (index >= m_chars.size()) {
        std::string x = "Index out of range: " + index;
        index = m_chars.size() - 1;
    }
//#endif
    return m_chars.at(index);
}

Color Settings::getRoundedColor(Color color) const {
    double value = colorValue(color);
    double max = maxValue();
    auto index = (size_t) (value * (double) (m_chars.size() - 1) / max);
    int remainder = value - static_cast<double>(index) * max / (double) (m_chars.size() - 1);

    Color roundedColor{color.getRed() - remainder / 3, color.getGreen() - remainder / 3,
                       color.getBlue() - (remainder / 3 + remainder % 3), color.getAlpha()};
    return roundedColor;

}
