#include "BasicFilter.h"

Color BasicFilter::apply(Color color, Vector, Frame &) const {
    return Color{static_cast<Color::number>((color.getRed() + m_red_offset) * m_red_factor),
                 static_cast<Color::number>((color.getGreen() + m_green_offset) * m_green_factor),
                 static_cast<Color::number>((color.getBlue() + m_blue_offset) * m_blue_factor),
                 static_cast<Color::number>((color.getAlpha() + m_alpha_offset) * m_alpha_factor)};
}


Color BasicFilter::apply(Color color, Vector) const {
    return Color{static_cast<Color::number>((color.getRed() + m_red_offset) * m_red_factor),
                 static_cast<Color::number>((color.getGreen() + m_green_offset) * m_green_factor),
                 static_cast<Color::number>((color.getBlue() + m_blue_offset) * m_blue_factor),
                 static_cast<Color::number>((color.getAlpha() + m_alpha_offset) * m_alpha_factor)};
}

BasicFilter::BasicFilter(double red_factor, double green_factor, double blue_factor, double alpha_factor,
                         double red_offset, double green_offset, double blue_offset, double alpha_offset) :
        m_red_factor(red_factor),
        m_green_factor(green_factor),
        m_blue_factor(blue_factor),
        m_alpha_factor(alpha_factor),
        m_red_offset(red_offset),
        m_green_offset(green_offset),
        m_blue_offset(blue_offset),
        m_alpha_offset(alpha_offset) {


}