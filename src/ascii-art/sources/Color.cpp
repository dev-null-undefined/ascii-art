#include "Color.h"
#if LOG_LEVEL >= 2
#include <iostream>
#endif

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

Color::Color(number m_r, number m_g, number m_b, number m_a) {
    setRed(m_r);
    setGreen(m_g);
    setBlue(m_b);
    setAlpha(m_a);
}

Color::number fixedNumber(Color::number number) {
#if LOG_LEVEL >= 2
    if(number > 255 || number < -255) {
        std::cerr << "Color::number fixedNumber(Color::number number): number is out of range: " << number << std::endl;
    }
#endif
    return number > 255 ? 255 : number < -255 ? -255 : number;
}

Color::number Color::getRed() const {
    return m_r;
}

void Color::setRed(number red) {
    m_r = fixedNumber(red);
}

Color::number Color::getGreen() const {
    return m_g;
}

void Color::setGreen(number green) {
    m_g = fixedNumber(green);
}

Color::number Color::getBlue() const {
    return m_b;
}

void Color::setBlue(number blue) {
    m_b = fixedNumber(blue);
}

Color::number Color::getAlpha() const {
    return m_a;
}

void Color::setAlpha(number alpha) {
    m_a = fixedNumber(alpha);
}

Color::number normalizeNumber(Color::number number) {
    return number > 255 ? 255 : number < 0 ? 0 : number;
}
Color Color::normalize() const {
    return Color{normalizeNumber(m_r), normalizeNumber(m_g), normalizeNumber(m_b), normalizeNumber(m_a)};
}
