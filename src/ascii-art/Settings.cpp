#include <string>
#include "Settings.h"
#include "logging/Logger.h"

double Settings::maxValue() const {
    return colorValue(Color{255, 255, 255, 255});
}

double Settings::colorValue(const Color & color) const {
    double value = 0;
    Color modified = m_brightness_filter.apply(color, {}).normalize();
    value += modified.getRed();
    value += modified.getGreen();
    value += modified.getBlue();
    value += modified.getAlpha();
    return value < 0 ? 0 : value;
}

Color Settings::modifiedColor(const Color & color) const {
    return m_color_filter.apply(color, {});
}


Settings::Settings() {
    std::string chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    m_chars = std::vector<char>(chars.begin(), chars.end());
}

char Settings::getChar(const Color & color) const {
    double value = colorValue(color.normalize());
    double max = maxValue();
    auto index = (size_t) (value * (double) (m_chars.size() - 1) / max);
#ifdef DEBUG
    if (index >= m_chars.size()) {
        Logger::log("Index out of range: " + std::to_string(index) + " " + std::to_string(m_chars.size()),LogLevel::ERROR);
        index = m_chars.size() - 1;
    }
#endif
    return m_chars.at(index);
}

Color Settings::getRoundedColor(Color color) const {
    double value = colorValue(color.normalize());
    double max = maxValue();
    auto index = (size_t) (value * (double) (m_chars.size() - 1) / max);
#ifdef DEBUG
    if (index >= m_chars.size()) {
        Logger::log("Index out of range: " + std::to_string(index) + " " + std::to_string(m_chars.size()),LogLevel::ERROR);
        index = m_chars.size() - 1;
    }
#endif
    int remainder = static_cast<int>(value - static_cast<double>(index) * max / (double) (m_chars.size() - 1));

    Color roundedColor{color.getRed() - remainder / 3, color.getGreen() - remainder / 3,
                       color.getBlue() - (remainder / 3 + remainder % 3), color.getAlpha()};
    return roundedColor;

}
