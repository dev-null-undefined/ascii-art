#include "Color.h"


Color & Color::operator+=(const Color & other) {

    m_r += other.m_r;
    m_g += other.m_g;
    m_b += other.m_b;
    m_a += other.m_a;
    return *this;
}

Color Color::operator-() const {
    return Color{-m_r, -m_g, -m_b, -m_a};
}

Color Color::operator+(const Color & other) const {

    return Color(*this) += other;
}

Color Color::operator-(const Color & other) const {

    return Color(*this) -= other;
}

Color & Color::operator-=(const Color & other) {

    return *this += -other;
}

Color & Color::operator*(Color::number factor) {

    m_r *= factor;
    m_g *= factor;
    m_b *= factor;
    m_a *= factor;
    return *this;
}

Color & Color::operator*(double factor) {
    m_r = static_cast<number>(static_cast<double>(m_r) * factor);
    m_g = static_cast<number>(static_cast<double>(m_g) * factor);
    m_b = static_cast<number>(static_cast<double>(m_b) * factor);
    m_a = static_cast<number>(static_cast<double>(m_a) * factor);
    return *this;
}

Color::Color(number m_r, number m_g, number m_b, number m_a) : m_r(m_r > 255 ? 255 : m_r),
                                                               m_g(m_g > 255 ? 255 : m_g),
                                                               m_b(m_b > 255 ? 255 : m_b),
                                                               m_a(m_a > 255 ? 255 : m_a) {}

Color::number Color::getRed() const {
    return m_r;
}

void Color::setRed(number red) {
    m_r = red > 255 ? 255 : red;
}

Color::number Color::getGreen() const {
    return m_g;
}

void Color::setGreen(number green) {
    m_g = green > 255 ? 255 : green;
}

Color::number Color::getBlue() const {
    return m_b;
}

void Color::setBlue(number blue) {
    m_b = blue > 255 ? 255 : blue;
}

Color::number Color::getAlpha() const {
    return m_a;
}

void Color::setAlpha(number alpha) {
    m_a = alpha > 255 ? 255 : alpha;
}
